################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/gameboy_spoof.c \
../Core/gen2.c \
../Core/gen3.c \
../Core/hardware.c 

OBJS += \
./Core/gameboy_spoof.o \
./Core/gen2.o \
./Core/gen3.o \
./Core/hardware.o 

C_DEPS += \
./Core/gameboy_spoof.d \
./Core/gen2.d \
./Core/gen3.d \
./Core/hardware.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o Core/%.su: ../Core/%.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core

clean-Core:
	-$(RM) ./Core/gameboy_spoof.d ./Core/gameboy_spoof.o ./Core/gameboy_spoof.su ./Core/gen2.d ./Core/gen2.o ./Core/gen2.su ./Core/gen3.d ./Core/gen3.o ./Core/gen3.su ./Core/hardware.d ./Core/hardware.o ./Core/hardware.su

.PHONY: clean-Core

