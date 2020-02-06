################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Example_EPwmSetup.obj" \
"./Example_posspeed.obj" \
"./F28M35x_CodeStartBranch.obj" \
"./F28M35x_CpuTimers.obj" \
"./F28M35x_DefaultIsr.obj" \
"./F28M35x_EPwm.obj" \
"./F28M35x_EQep.obj" \
"./F28M35x_GlobalVariableDefs.obj" \
"./F28M35x_PieCtrl.obj" \
"./F28M35x_PieVect.obj" \
"./F28M35x_SysCtrl.obj" \
"./eqep_pos_speed_c28.obj" \
"C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/cmd/F28M35x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/cmd/F28M35x_generic_wshared_C28_FLASH.cmd" \
-lrts2800_ml.lib \
-lIQmath.lib \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
eqep_pos_speed_c28.out \

EXE_OUTPUTS__QUOTED += \
"eqep_pos_speed_c28.out" \

BIN_OUTPUTS += \
eqep_pos_speed_c28.hex \

BIN_OUTPUTS__QUOTED += \
"eqep_pos_speed_c28.hex" \


# All Target
all: eqep_pos_speed_c28.out

# Tool invocations
eqep_pos_speed_c28.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" -z -m"eqep_pos_speed_c28.map" --stack_size=0x300 --warn_sections -i"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/lib" -i"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" -i"C:/ti/controlSUITE/libs/math/IQmath/v160/lib" -i"C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs" -i"C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --reread_libs --xml_link_info="eqep_pos_speed_c28_linkInfo.xml" --entry_point=code_start --rom_model -o "eqep_pos_speed_c28.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

eqep_pos_speed_c28.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/hex2000"  -o "eqep_pos_speed_c28.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "Example_EPwmSetup.d" "Example_posspeed.d" "F28M35x_CpuTimers.d" "F28M35x_DefaultIsr.d" "F28M35x_EPwm.d" "F28M35x_EQep.d" "F28M35x_GlobalVariableDefs.d" "F28M35x_PieCtrl.d" "F28M35x_PieVect.d" "F28M35x_SysCtrl.d" "eqep_pos_speed_c28.d" 
	-$(RM) "Example_EPwmSetup.obj" "Example_posspeed.obj" "F28M35x_CodeStartBranch.obj" "F28M35x_CpuTimers.obj" "F28M35x_DefaultIsr.obj" "F28M35x_EPwm.obj" "F28M35x_EQep.obj" "F28M35x_GlobalVariableDefs.obj" "F28M35x_PieCtrl.obj" "F28M35x_PieVect.obj" "F28M35x_SysCtrl.obj" "eqep_pos_speed_c28.obj" 
	-$(RM) "F28M35x_CodeStartBranch.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

