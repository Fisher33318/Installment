################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
enet_uip.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Master/enet_uip/m3/enet_uip.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Master/enet_uip/m3" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0/uip" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0/apps" -g --gcc --define=ccs --define=_STANDALONE --define="_FLASH" --define=PART_LM3S9B96 --diag_suppress=10199 --diag_warning=225 --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="enet_uip.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

httpd.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Master/enet_uip/m3/httpd.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Master/enet_uip/m3" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0/uip" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0/apps" -g --gcc --define=ccs --define=_STANDALONE --define="_FLASH" --define=PART_LM3S9B96 --diag_suppress=10199 --diag_warning=225 --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="httpd.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

set_pinout_f28m35x.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/board_drivers/set_pinout_f28m35x.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Master/enet_uip/m3" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0/uip" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0/apps" -g --gcc --define=ccs --define=_STANDALONE --define="_FLASH" --define=PART_LM3S9B96 --diag_suppress=10199 --diag_warning=225 --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="set_pinout_f28m35x.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Master/enet_uip/m3/startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Master/enet_uip/m3" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0/uip" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/MWare/third_party/uip-1.0/apps" -g --gcc --define=ccs --define=_STANDALONE --define="_FLASH" --define=PART_LM3S9B96 --diag_suppress=10199 --diag_warning=225 --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="startup_ccs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


