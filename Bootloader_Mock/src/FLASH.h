#ifndef __FLASH_H__
#define __FLASH_H__

/*
 * =========================================================================
 * Includes
 * =========================================================================
 */
#include <stdint.h>

/*
 * =========================================================================
 * Macros & Defines
 * =========================================================================
 */
#define CMD_PROGRAM_LONGWORD        (0x07)
#define CMD_ERASE_FLASH_SECTOR      (0x09)

/* Program alignment */
#define FTFC_WRITE_DOUBLE_WORD      (8U)
#define FTFC_P_FLASH_SECTOR_SIZE    (0x1000)
#define WRITE_FUNCTION_ADDRESS      (0x1FFF8400)


/*
 * =========================================================================
 * Private / Internal Functions
 * =========================================================================
 */
void Mem_43_INFLS_IPW_LoadAc(void);

void Ftfc_AccessCode(void) __attribute__ ((section (".acmem_43_infls_code_rom")));


/*
 * =========================================================================
 * API Functions
 * =========================================================================
 */

/*
 * Read_FlashAddress
 * Get address
 * - Addr: input address
 * - returns: return address
 */
uint32_t Read_FlashAddress(uint32_t Addr);

/*
 * Program_LongWord_8B
 * Flash data input into flash
 * - Addr: address to flash data to flash
 * - Data: input data need to flash data into flash
 * - returns: 1 if success
 */
uint8_t Program_LongWord_8B(uint32_t Addr, uint8_t *Data);

/*
 * Erase_Sector
 * Erase a sector in flash
 * - Addr: address to erase
 * - returns: 1 if success
 */
uint8_t Erase_Sector(uint32_t Addr);

/*
 * Erase_Multi_Sector
 * Erase multi sectors in flash
 * - Addr: address to erase
 * - Size: number of sectors to erase
 * - returns: 1 if success
 */
uint8_t Erase_Multi_Sector(uint32_t Addr, uint8_t Size);

#endif /* __FLASH_H__ */
