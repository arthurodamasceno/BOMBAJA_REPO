################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Drivers/Startup/startup_stm32f103c8tx.s 

OBJS += \
./Drivers/Startup/startup_stm32f103c8tx.o 

S_DEPS += \
./Drivers/Startup/startup_stm32f103c8tx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Startup/startup_stm32f103c8tx.o: ../Drivers/Startup/startup_stm32f103c8tx.s
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -I"D:/BOMBAJA_REPO/Rear_ECU/Middlewares/Third_Party/SEGGER/Config" -x assembler-with-cpp -MMD -MP -MF"Drivers/Startup/startup_stm32f103c8tx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

