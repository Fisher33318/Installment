################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Example_EPwmSetup.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/Example_EPwmSetup.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="Example_EPwmSetup.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Example_posspeed.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/Example_posspeed.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="Example_posspeed.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_CodeStartBranch.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/source/F28M35x_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_CodeStartBranch.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_CpuTimers.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/source/F28M35x_CpuTimers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_CpuTimers.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_DefaultIsr.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/source/F28M35x_DefaultIsr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_DefaultIsr.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_EPwm.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/source/F28M35x_EPwm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_EPwm.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_EQep.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/source/F28M35x_EQep.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_EQep.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_GlobalVariableDefs.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/source/F28M35x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_GlobalVariableDefs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_PieCtrl.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/source/F28M35x_PieCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_PieCtrl.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_PieVect.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/source/F28M35x_PieVect.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_PieVect.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

F28M35x_SysCtrl.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/source/F28M35x_SysCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="F28M35x_SysCtrl.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

eqep_pos_speed_c28.obj: C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/eqep_pos_speed_c28.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/bin/cl2000" -v28 -ml -mt --vcu_support=vcu0 --include_path="C:/ti/ccsv6/tools/compiler/c2000_15.12.3.LTS/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_common/include" -g --define="_DEBUG" --define="_FLASH" --define="LARGE_MODEL" --quiet --display_error_number --diag_suppress=10063 --diag_suppress=16002 --diag_warning=225 --issue_remarks --no_fast_branch --output_all_syms --asm_directory="C:/ti/controlSUITE/device_support/f28m35x/v207/F28M35x_examples_Control/eqep_pos_speed/c28/ccs/Debug" --preproc_with_compile --preproc_dependency="eqep_pos_speed_c28.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


