file(GLOB CM_KERNEL *.cpp)
file(GLOB OCL_KERNEL *.cl)

foreach (KERNEL ${CM_KERNEL})
    get_filename_component(KERNEL_NAME ${KERNEL} NAME_WE)
    message(${KERNEL_NAME})
    set(BIN ${KERNEL_NAME}_L0.bin)
    set(KERNEL_TARGET ${KERNEL_NAME}_L0)
    add_custom_target(${KERNEL_TARGET}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
            ${BIN}
        COMMENT
            "linking the l0 kernel to bin file!"
        VERBATIM
    )
    add_custom_command(
        OUTPUT
            ${BIN}
        COMMAND
            cmoc -Qxcm_jit_option="-noverifyCISA" -Qxcm_jit_option=-noLocalSplit
            -Qxcm_jit_option=-enableBCR -Qxcm_jit_option=-SWSBDepReduction
            -Qxcm_jit_option=-ratrace -Qxcm_print_asm_count
            -mCM_printregusage
            -mCM_emit_common_isa
            -mCM_no_vector_decomposition /std:c++17 -D CM_GEN12_7
            -mdump_asm -march=pvcxt_b0 -m64
            -binary-format=ze
            -o ${BIN}
            -- ${KERNEL}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
            ${KERNEL}
        COMMENT
            "compile the l0 kernel to bin file!"
    )
    add_dependencies(${TARGET_NAME} ${KERNEL_TARGET})
endforeach ()

foreach (KERNEL ${OCL_KERNEL})
    get_filename_component(KERNEL_NAME ${KERNEL} NAME_WE)
    message(${KERNEL_NAME})
    set(BIN ${KERNEL_NAME}_OCL.bin)
    set(KERNEL_TARGET ${KERNEL_NAME}_OCL)
    add_custom_target(${KERNEL_TARGET}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
            ${BIN}
        COMMENT
            "linking the ocl kernel to bin file!"
        VERBATIM
    )
    add_custom_command(
        OUTPUT
            ${BIN}
        COMMAND
            ocloc -file ${KERNEL} -device pvc
            -internal_options "-allow-zebin -dumpvisa -dump-isa-binary"
            -output_no_suffix
            -output ${BIN}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
            ${KERNEL}
        COMMENT
            "compile the ocl kernel to bin file!"
    )
    add_dependencies(${TARGET_NAME} ${KERNEL_TARGET})
endforeach ()