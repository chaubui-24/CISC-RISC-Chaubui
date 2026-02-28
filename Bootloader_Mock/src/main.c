/*
 * =========================================================================
 * main.c
 * Bootloader Application (BOOT APP)
 * =========================================================================
 */

#include "S32K144.h"
#include "Driver_USART.h"
#include "Driver_GPIO.h"
#include "QUEUE.h"
#include "SREC.h"
#include <s32_core_cm4.h>
#include <string.h>
#include "FLASH.h"
#include <stdint.h>

/*
 * =========================================================================
 * Macros & Defines
 * =========================================================================
 */

/* Start address of USER APP */
#define USER_APP_START_ADDR     (0x0000A000UL)

/* Boot Button is pin PTC13 (Port C, Pin 13) */
#define BOOT_BUTTON_PIN         (13U)

/* Buffer definition */
#define APP_BUFFER_SIZE         (100U)

/* Erase size: 118 sectors from 0xA000 */
#define ERASE_SECTOR_COUNT      (118U)

/*
 * =========================================================================
 * Variables
 * =========================================================================
 */

/* Global variables */
uint8_t g_appBuffer[APP_BUFFER_SIZE];
SREC_Record_t g_srecLineData;

/* External drivers */
extern ARM_DRIVER_USART Driver_USART0;
extern ARM_DRIVER_GPIO  Driver_GPIO1; /* GPIO1 manages Port C */

/* Static variables */
/* Buffer for handling 4-byte line merging */
static uint8_t  s_pendingBuffer[8U];     /* 8-byte buffer for merging */
static uint32_t s_pendingAddr = 0U;      /* Address of the first 4 bytes */
static uint8_t  s_hasPendingData = 0U;   /* Flag: Is there half a word waiting? */

static uint8_t  s_inputSrecType = 0U;    /* Hold the value of the SREC type that contains data*/
static uint8_t  s_flagFirstSrecType = 0U;/* Flag: is this the first ever line of the SREC type */

/*
 * =========================================================================
 * Prototypes
 * =========================================================================
 */
typedef void (*pFunction)(void);

static inline void My_Set_MSP       (uint32_t topOfMainStack);
void               Process_Smart_Write(uint32_t addr, uint8_t *data, uint32_t len);
void               Flush_Pending_Data (void);
void               SOSC_init_8MHz     (void);
void               SPLL_init_160MHz   (void);
void               NormalRunMode_Init (void);
void               JumpToUserApp      (uint32_t start_addr);
void               SREC_ReportError   (SREC_Status_t error_code);

/*
 * =========================================================================
 * Main Function
 * =========================================================================
 */
int main(void)
{
    /* Local variables definition */
    int32_t status = 0;
    SREC_Status_t parseStatus = SREC_OK;
    uint32_t remaining = 0U;
    uint32_t offset = 0U;
    uint32_t chunkSize = 0U;

    /* 1. System Clock Initialization */
    SOSC_init_8MHz();
    SPLL_init_160MHz();
    NormalRunMode_Init();

    /* 2. IMPORTANT: Copy Flash execution function to RAM */
    /* This function is in FLASH.c, must be called before any erase/write operation */
    Mem_43_INFLS_IPW_LoadAc();

    /* 3. Enable Global Interrupts (for GPIO and UART operation) */
    ENABLE_INTERRUPTS();

    /* 4. Check Boot Button (PTC13) */
    Driver_GPIO1.Setup(BOOT_BUTTON_PIN, NULL);
    Driver_GPIO1.SetDirection(BOOT_BUTTON_PIN, ARM_GPIO_INPUT);

    /* Logic:
       - Assume Pull-up resistor.
       - No Press = Logic 1 (High).
       - Press = Logic 0 (Low).
    */

    /* If NOT PRESSED (Input == 1) -> Check and jump to User App */
    if (Driver_GPIO1.GetInput(BOOT_BUTTON_PIN) != 0U)
    {
        /* Check if application address has valid data (Not 0xFFFFFFFF) */
        /* Only check MSP (first 4 bytes), can check checksum if needed */
        if (Read_FlashAddress(USER_APP_START_ADDR) != 0xFFFFFFFFUL)
        {
            JumpToUserApp(USER_APP_START_ADDR);
        }
        /* If User App is empty, program flows down to run Bootloader */
    }

    /* --- BOOTLOADER MODE (Due to button press or empty App) --- */

    Driver_USART0.Initialize(NULL);
    Driver_USART0.Send((const uint8_t *)"\r\n=== BOOT APP MODE ===\r\n", 0U);

    /* 5. Erase User App memory region before writing new one */
    Driver_USART0.Send((const uint8_t *)">> Erasing Flash... ", 0U);

    /* Disable interrupts when erasing Flash to prevent CPU hang (ISR in Flash) */
    DISABLE_INTERRUPTS();
    Erase_Multi_Sector(USER_APP_START_ADDR, ERASE_SECTOR_COUNT); /* Erase from 0xA000 to end */
    ENABLE_INTERRUPTS();

    Driver_USART0.Send((const uint8_t *)"Done.\r\n", 0U);
    Driver_USART0.Send((const uint8_t *)">> Request: Please send SREC file now...\r\n", 0U);

    while (1)
    {
        /* Receive data from Queue */
        status = Driver_USART0.Receive(g_appBuffer, APP_BUFFER_SIZE);

        if (status == (int32_t)ARM_DRIVER_OK)
        {
            /* 1. Call Parse function and store result */
            parseStatus = SREC_ParseLine(g_appBuffer, &g_srecLineData);

            /* 2. Check result */
            if (parseStatus == SREC_OK)
            {
                if ((g_srecLineData.type >= 1U) && (g_srecLineData.type <= 3U))
                {
                    if (s_flagFirstSrecType == 0U)
                    {
                        s_inputSrecType = g_srecLineData.type;
                        s_flagFirstSrecType++;
                    }

                    if (g_srecLineData.type != s_inputSrecType)
                    {
                        Driver_USART0.Send((const uint8_t *)"\r\n>> System Halted: Duplicated Types Detected.\r\n", 0U);
                        Driver_USART0.Send((const uint8_t *)"\r\n>> Erasing Flash... ", 0U);
                        DISABLE_INTERRUPTS();
                        Erase_Multi_Sector(USER_APP_START_ADDR, ERASE_SECTOR_COUNT);
                        ENABLE_INTERRUPTS();
                        Driver_USART0.Send((const uint8_t *)"Done\r\n", 0U);
                        Driver_USART0.Send((const uint8_t *)">> Request: Please reset the board\r\n", 0U);

                        /* Infinite loop to halt system */
                        while (1)
                        {
                            /* Do nothing */
                        }
                    }

                    /* Instead of complex for loop, pass directly to smart process function */
                    /* Note: Process_Smart_Write currently supports input of 4 or 8 bytes */
                    remaining = g_srecLineData.data_length;
                    offset = 0U;

                    /* Cut SREC line into small chunks (4 or 8 bytes) */
                    /* Usually standard SREC is 16 bytes -> Cut into 8 + 8 */
                    while (remaining > 0U)
                    {
                        /* Prioritize cutting 8-byte chunks if enough */
                        chunkSize = (remaining >= 8U) ? 8U : 4U;

                        /* If remaining is odd (e.g., 6 bytes), logic needs refinement,
                           but S32K usually encounters 4/8/16 bytes */
                        Process_Smart_Write(g_srecLineData.address + offset,
                                            &g_srecLineData.data[offset],
                                            chunkSize);

                        remaining -= chunkSize;
                        offset += chunkSize;
                    }
                }

                /* End of file (S7, S8, S9) */
                if ((g_srecLineData.type >= 7U) && (g_srecLineData.type <= 9U))
                {
                    /* IMPORTANT: Flush the final 4-byte chunk (if any) */
                    Flush_Pending_Data();

                    Driver_USART0.Send((const uint8_t *)"\r\n>> Download Complete!\r\n", 0U);
                    Driver_USART0.Send((const uint8_t *)"\r\n>> Request: Press the RESET button to run UserApp!\r\n", 0U);
                }
            }
            else
            {
                /* --- ERROR CASE --- */
                /* Report specific error to screen */
                SREC_ReportError(parseStatus);
                Driver_USART0.Send((const uint8_t *)"\r\n>> Erasing Flash... ", 0U);
                DISABLE_INTERRUPTS();
                Erase_Multi_Sector(USER_APP_START_ADDR, ERASE_SECTOR_COUNT);
                ENABLE_INTERRUPTS();
                Driver_USART0.Send((const uint8_t *)"Done.\r\n", 0U);
                Driver_USART0.Send((const uint8_t *)">> Request: Please reset the board\r\n", 0U);

                /* Stop program immediately (Infinite Loop) */
                /* User must press Reset button on board to restart */
                while (1)
                {
                    /* Could add Red LED toggling here to indicate error */
                }
            }
        }
    }

    return 0;
}

/*
 * =========================================================================
 * API Implementation
 * =========================================================================
 */

/**
 * @brief Setup Main Stack Pointer (MSP) for User App
 */
static inline void My_Set_MSP(uint32_t topOfMainStack)
{
    __asm volatile ("MSR msp, %0" : : "r" (topOfMainStack) : "memory");
}

/* Logic to handle line merging based on length */
void Process_Smart_Write(uint32_t addr, uint8_t *data, uint32_t len)
{
    /* Flag to indicate if we need to process the current data as new after flushing old data */
    uint8_t processCurrentAsNew = 0U;

    /* CASE 1: Pending data exists (Holding a previous 4-byte chunk) */
    if (s_hasPendingData == 1U)
    {
        /* Check: Is the new line the next piece? */
        /* Condition: Address is consecutive (pending + 4) */
        if (addr == (s_pendingAddr + 4U))
        {
            /* --- MATCH! PROCEED TO MERGE --- */
            /* 1. Fill the hole: Copy first 4 bytes of new data to second half of buffer */
            memcpy(&s_pendingBuffer[4U], data, 4U);

            /* 2. Write the now complete 8-byte block to the ALIGNED address */
            DISABLE_INTERRUPTS();
            Program_LongWord_8B(s_pendingAddr, s_pendingBuffer);
            ENABLE_INTERRUPTS();

            /* 3. Handle the leftovers if input was 8 bytes */
            if (len == 8U)
            {
                /* The input had 8 bytes, we only used 4. */
                /* The remaining 4 bytes become the NEW pending data */
                memcpy(s_pendingBuffer, &data[4U], 4U);

                /* Update pending address to the next slot */
                s_pendingAddr = addr + 4U;

                /* Flag remains 1 because we still have pending data */
                s_hasPendingData = 1U;
            }
            else
            {
                /* Input was exactly 4 bytes. Perfect fit, no leftovers. */
                s_hasPendingData = 0U;
            }

            /* We handled the data, do not treat as new */
            processCurrentAsNew = 0U;
        }
        else
        {
            /* --- MISMATCH (DISCONTINUOUS) --- */
            /* 1. Must write the old pending line first (Padding FF at the end) */
            memset(&s_pendingBuffer[4U], 0xFF, 4U); /* Fill FF in missing part */
            DISABLE_INTERRUPTS();
            Program_LongWord_8B(s_pendingAddr, s_pendingBuffer);
            ENABLE_INTERRUPTS();

            s_hasPendingData = 0U; /* Released pending data */

            /* 2. Mark to process the current data as a fresh start */
            processCurrentAsNew = 1U;
        }
    }
    else
    {
        /* No pending data, treat as new */
        processCurrentAsNew = 1U;
    }

    /* CASE 2: Process current data as new (Standard Logic) */
    if (processCurrentAsNew == 1U)
    {
        if (len == 8U)
        {
            /* If standard 8 bytes: Write immediately */
            DISABLE_INTERRUPTS();
            Program_LongWord_8B(addr, data);
            ENABLE_INTERRUPTS();
        }
        else if (len == 4U)
        {
            /* If odd 4 bytes: STORE IN WAREHOUSE */
            memcpy(s_pendingBuffer, data, 4U); /* Save to first half */
            s_pendingAddr = addr;              /* Save address */
            s_hasPendingData = 1U;             /* Set flag */
        }
        else
        {
            /* Other lengths ignored */
        }
    }
}

/* Final function call to flush remaining data (if SREC file ends oddly) */
void Flush_Pending_Data(void)
{
    if (s_hasPendingData == 1U)
    {
        /* Fill FF at the end and write */
        memset(&s_pendingBuffer[4U], 0xFF, 4U);
        DISABLE_INTERRUPTS();
        Program_LongWord_8B(s_pendingAddr, s_pendingBuffer);
        ENABLE_INTERRUPTS();
        s_hasPendingData = 0U;
    }
}

/* Clock Initializations */
void SOSC_init_8MHz(void)
{
    IP_SCG->SOSCDIV = 0x00000101U;
    IP_SCG->SOSCCFG = SCG_SOSCCFG_RANGE(0x02U) | SCG_SOSCCFG_HGO(0x00U) | SCG_SOSCCFG_EREFS(0x01U);

    while ((IP_SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK) != 0U)
    {
        /* Wait for lock */
    }

    IP_SCG->SOSCCSR |= SCG_SOSCCSR_SOSCEN(0x01U);

    while ((IP_SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) == 0U)
    {
        /* Wait for valid */
    }
}

void SPLL_init_160MHz(void)
{
    IP_SCG->SPLLDIV = 0x00000302U;
    IP_SCG->SPLLCFG = SCG_SPLLCFG_MULT(0x18U) | SCG_SPLLCFG_PREDIV(0x00U);

    while ((IP_SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK) != 0U)
    {
        /* Wait for lock */
    }

    IP_SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN(0x01U);

    while ((IP_SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK) == 0U)
    {
        /* Wait for valid */
    }
}

void NormalRunMode_Init(void)
{
    IP_SCG->RCCR = SCG_RCCR_SCS(0x06U) | SCG_RCCR_DIVCORE(0x01U) | SCG_RCCR_DIVBUS(0x03U) | SCG_RCCR_DIVSLOW(0x05U);

    while (((IP_SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != 6U)
    {
        /* Wait for clock switch */
    }
}

/* Function to jump to User App */
void JumpToUserApp(uint32_t start_addr)
{
    /* 1. Get Reset Handler address from User App Vector Table */
    uint32_t jumpAddress = 0U;
    pFunction jumpToApp = NULL;

    jumpAddress = Read_FlashAddress(start_addr + 4U);
    jumpToApp = (pFunction)jumpAddress;

    /* 2. Disable all interrupts to ensure safe jump */
    DISABLE_INTERRUPTS();

    /* 3. Setup new MSP (Main Stack Pointer) */
    My_Set_MSP(Read_FlashAddress(start_addr));

    /* 4. Jump! */
    jumpToApp();
}

/* Utility function to report specific SREC errors */
void SREC_ReportError(SREC_Status_t error_code)
{
    const uint8_t *msg = (const uint8_t *)"Unknown Error";

    switch (error_code)
    {
        case SREC_ERROR_NULL_PTR:
            msg = (const uint8_t *)"ERROR: Null Pointer detected!";
            break;
        case SREC_ERROR_TYPE:
            msg = (const uint8_t *)"ERROR: Invalid S-Record Type (Start char not 'S')!";
            break;
        case SREC_ERROR_FORMAT:
            msg = (const uint8_t *)"ERROR: Invalid Format (Length mismatch)!";
            break;
        case SREC_ERROR_CHECKSUM:
            msg = (const uint8_t *)"ERROR: Checksum Mismatch!";
            break;
        default:
            /* Default case */
            break;
    }

    Driver_USART0.Send((const uint8_t *)"\r\n>> FATAL ERROR: ", 0U);
    Driver_USART0.Send(msg, 0U);
}
