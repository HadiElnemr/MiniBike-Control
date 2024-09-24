################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../WHEELTEC/DataScope_DP.c \
../WHEELTEC/MPU6050.c \
../WHEELTEC/control.c \
../WHEELTEC/inv_mpu.c \
../WHEELTEC/inv_mpu_dmp_motion_driver.c \
../WHEELTEC/show.c 

OBJS += \
./WHEELTEC/DataScope_DP.o \
./WHEELTEC/MPU6050.o \
./WHEELTEC/control.o \
./WHEELTEC/inv_mpu.o \
./WHEELTEC/inv_mpu_dmp_motion_driver.o \
./WHEELTEC/show.o 

C_DEPS += \
./WHEELTEC/DataScope_DP.d \
./WHEELTEC/MPU6050.d \
./WHEELTEC/control.d \
./WHEELTEC/inv_mpu.d \
./WHEELTEC/inv_mpu_dmp_motion_driver.d \
./WHEELTEC/show.d 


# Each subdirectory must supply rules for building sources it contributes
WHEELTEC/%.o WHEELTEC/%.su: ../WHEELTEC/%.c WHEELTEC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/Administrator/Desktop/WHEELTEC/Core/Inc" -I"C:/Users/Administrator/Desktop/WHEELTEC/BSPcode/Inc" -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Administrator/Desktop/WHEELTEC/WHEELTEC/Inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-WHEELTEC

clean-WHEELTEC:
	-$(RM) ./WHEELTEC/DataScope_DP.d ./WHEELTEC/DataScope_DP.o ./WHEELTEC/DataScope_DP.su ./WHEELTEC/MPU6050.d ./WHEELTEC/MPU6050.o ./WHEELTEC/MPU6050.su ./WHEELTEC/control.d ./WHEELTEC/control.o ./WHEELTEC/control.su ./WHEELTEC/inv_mpu.d ./WHEELTEC/inv_mpu.o ./WHEELTEC/inv_mpu.su ./WHEELTEC/inv_mpu_dmp_motion_driver.d ./WHEELTEC/inv_mpu_dmp_motion_driver.o ./WHEELTEC/inv_mpu_dmp_motion_driver.su ./WHEELTEC/show.d ./WHEELTEC/show.o ./WHEELTEC/show.su

.PHONY: clean-WHEELTEC

