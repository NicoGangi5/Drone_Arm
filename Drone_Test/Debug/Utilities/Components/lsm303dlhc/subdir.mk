################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Components/lsm303dlhc/lsm303dlhc.c 

OBJS += \
./Utilities/Components/lsm303dlhc/lsm303dlhc.o 

C_DEPS += \
./Utilities/Components/lsm303dlhc/lsm303dlhc.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Components/lsm303dlhc/%.o: ../Utilities/Components/lsm303dlhc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/ili9325" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/s25fl512s" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/cs43l22" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/ili9341" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/ampire480272" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/n25q512a" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/s5k5cag" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/mfxstm32l152" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/CMSIS/device" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/ts3510" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/n25q128a" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/st7735" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/HAL_Driver/Inc/Legacy" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/lis302dl" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/otm8009a" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/stmpe1600" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/ov2640" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/Common" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/l3gd20" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/HAL_Driver/Inc" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/stmpe811" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/lis3dsh" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/wm8994" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/n25q256a" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/inc" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/ls016b8uy" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/ft6x06" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/STM32F4-Discovery" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/exc7200" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/st7789h2" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/ampire640480" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/Utilities/Components/lsm303dlhc" -I"/Users/NicoGangi/Documents/workspace/Drone_Test/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


