################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ORG1411_GPS/ORG1411_GPS.c 

OBJS += \
./ORG1411_GPS/ORG1411_GPS.o 

C_DEPS += \
./ORG1411_GPS/ORG1411_GPS.d 


# Each subdirectory must supply rules for building sources it contributes
ORG1411_GPS/ORG1411_GPS.o: ../ORG1411_GPS/ORG1411_GPS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L432xx -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ORG1411_GPS/ORG1411_GPS.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

