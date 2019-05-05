################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/opt/ti/ccs9/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/bin/cl2000" -v28 -ml -mt --include_path="D:/workspace/CCS_V9/Q16_FINGERPRINT_MOTOR_BEE_F2812_CCS8_0" --include_path="../inc" --include_path="../DSP281x_common/include" --include_path="../DSP281x_headers/include" --include_path="C:/opt/ti/ccs9/ccs/tools/compiler/ti-cgt-c2000_18.12.1.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


