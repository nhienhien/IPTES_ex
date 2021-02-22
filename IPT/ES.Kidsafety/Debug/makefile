################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ES.Kidsafety.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ES.Kidsafety.list \

OBJCOPY_BIN += \
ES.Kidsafety.bin \


# All Target
all: ES.Kidsafety.elf secondary-outputs

# Tool invocations
ES.Kidsafety.elf: $(OBJS) $(USER_OBJS) E:\Dowload\Tailieu-Baigiang\stm32\IPTES\ES.Kidsafety\STM32L432KCUX_FLASH.ld
	arm-none-eabi-gcc -o "ES.Kidsafety.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"E:\Dowload\Tailieu-Baigiang\stm32\IPTES\ES.Kidsafety\STM32L432KCUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ES.Kidsafety.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ES.Kidsafety.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ES.Kidsafety.list"
	@echo 'Finished building: $@'
	@echo ' '

ES.Kidsafety.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ES.Kidsafety.bin"
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
