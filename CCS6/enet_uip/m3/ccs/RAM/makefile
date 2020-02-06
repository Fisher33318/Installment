################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./enet_uip.obj" \
"./httpd.obj" \
"./set_pinout_f28m35x.obj" \
"./startup_ccs.obj" \
"./third_party/uip-1.0/apps/dhcpc/dhcpc.obj" \
"./third_party/uip-1.0/uip/uip.obj" \
"./third_party/uip-1.0/uip/uip_arp.obj" \
"./third_party/uip-1.0/uip/uip_timer.obj" \
"./utils/uartstdio.obj" \
"./utils/ustdlib.obj" \
"C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/driverlib/ccs/Debug/driverlib.lib" \
$(GEN_CMDS__FLAG) \
-lrtsv7M3_T_le_eabi.lib \
-l"C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/cmd/F28M35x_generic_M3_RAM.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include third_party/uip-1.0/apps/dhcpc/subdir_vars.mk
-include third_party/uip-1.0/uip/subdir_vars.mk
-include utils/subdir_vars.mk
-include subdir_rules.mk
-include third_party/uip-1.0/apps/dhcpc/subdir_rules.mk
-include third_party/uip-1.0/uip/subdir_rules.mk
-include utils/subdir_rules.mk
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
enet_uip_m3.out \

EXE_OUTPUTS__QUOTED += \
"enet_uip_m3.out" \


# All Target
all: enet_uip_m3.out

# Tool invocations
enet_uip_m3.out: $(OBJS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --gcc --define=ccs --define=PART_LM3S9B96 --diag_suppress=10199 --diag_warning=225 --gen_func_subsections=on --ual -z -m"enet_uip_m3.map" --heap_size=0 --stack_size=1024 -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -i"C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Master/enet_uip/m3" --reread_libs --warn_sections --xml_link_info="enet_uip_m3_linkInfo.xml" --rom_model -o "enet_uip_m3.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "enet_uip.d" "httpd.d" "set_pinout_f28m35x.d" "startup_ccs.d" "third_party\uip-1.0\apps\dhcpc\dhcpc.d" "third_party\uip-1.0\uip\uip.d" "third_party\uip-1.0\uip\uip_arp.d" "third_party\uip-1.0\uip\uip_timer.d" "utils\uartstdio.d" "utils\ustdlib.d" 
	-$(RM) "enet_uip.obj" "httpd.obj" "set_pinout_f28m35x.obj" "startup_ccs.obj" "third_party\uip-1.0\apps\dhcpc\dhcpc.obj" "third_party\uip-1.0\uip\uip.obj" "third_party\uip-1.0\uip\uip_arp.obj" "third_party\uip-1.0\uip\uip_timer.obj" "utils\uartstdio.obj" "utils\ustdlib.obj" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"C:/ti/ccsv6/utils/tiobj2bin/tiobj2bin.bat" "enet_uip_m3.out" "enet_uip_m3.bin" "C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/ofd470.exe" "C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/hex470.exe" "C:/ti/ccsv6/utils/tiobj2bin/mkhex4bin.exe"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

