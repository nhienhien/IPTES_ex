################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include X-CUBE-AI/App/subdir.mk
-include Drivers/STM32_AI_AudioPreprocessing_Library/Src/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
CMCIS-test.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
CMCIS-test.list \

OBJCOPY_BIN += \
CMCIS-test.bin \


# All Target
all: CMCIS-test.elf secondary-outputs

# Tool invocations
CMCIS-test.elf: $(OBJS) $(USER_OBJS) C:\Users\Admin\Desktop\CMCIS-test_ham\CMCIS-test\CMCIS-test\CMCIS-test\STM32F746NGHX_FLASH.ld
	arm-none-eabi-gcc -o "CMCIS-test.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\Admin\Desktop\CMCIS-test_ham\CMCIS-test\CMCIS-test\CMCIS-test\STM32F746NGHX_FLASH.ld" --specs=nosys.specs -Wl,-Map="CMCIS-test.map" -Wl,--gc-sections -static -L"../libs" -L../Middlewares/ST/AI/Lib --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

CMCIS-test.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "CMCIS-test.list"
	@echo 'Finished building: $@'
	@echo ' '

CMCIS-test.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "CMCIS-test.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
