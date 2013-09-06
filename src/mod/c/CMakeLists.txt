project(MODC)

set(CMAKE_C_FLAGS "-g3 -Wall -Wextra -Werror -Wstrict-aliasing -std=c99 -pedantic")

add_library(mod_c SHARED c.c)

add_custom_command(TARGET mod_c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${MODC_SOURCE_DIR}/test_c.mod"
    $<TARGET_FILE_DIR:sharedobj>)
