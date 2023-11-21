function(generate_output_binaries TGT)
    add_custom_command(TARGET ${TGT} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TGT}>
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${TGT}> ${TGT}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TGT}> ${TGT}.bin)
endfunction()

function(clean_output_binaries TGT)
    set_target_properties(${TGT} PROPERTIES
        ADDITIONAL_CLEAN_FILES
        "${TGT}.map;${TGT}.bin;${TGT}.hex")
    set_target_properties(${TGT} PROPERTIES
        SUFFIX .elf)
endfunction()
