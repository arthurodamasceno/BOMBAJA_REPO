################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/SEGGER/Rec/segger_uart.c 

OBJS += \
./Middlewares/Third_Party/SEGGER/Rec/segger_uart.o 

C_DEPS += \
./Middlewares/Third_Party/SEGGER/Rec/segger_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/SEGGER/Rec/segger_uart.o: ../Middlewares/Third_Party/SEGGER/Rec/segger_uart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I"D:/BOMBAJA_REPO/Front_ECU/Middlewares/Third_Party/SEGGER/Config" -I"D:/BOMBAJA_REPO/Front_ECU/Middlewares/Third_Party/SEGGER/OS" -I"D:/BOMBAJA_REPO/Front_ECU/Middlewares/Third_Party/SEGGER/SEGGER" -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/SEGGER/Rec/segger_uart.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

