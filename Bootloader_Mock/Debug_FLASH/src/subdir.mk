################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Driver_GPIO.c \
../src/Driver_USART.c \
../src/FLASH.c \
../src/HAL_GPIO.c \
../src/HAL_USART.c \
../src/QUEUE.c \
../src/SREC.c \
../src/main.c 

OBJS += \
./src/Driver_GPIO.o \
./src/Driver_USART.o \
./src/FLASH.o \
./src/HAL_GPIO.o \
./src/HAL_USART.o \
./src/QUEUE.o \
./src/SREC.o \
./src/main.o 

C_DEPS += \
./src/Driver_GPIO.d \
./src/Driver_USART.d \
./src/FLASH.d \
./src/HAL_GPIO.d \
./src/HAL_USART.d \
./src/QUEUE.d \
./src/SREC.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Driver_GPIO.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


