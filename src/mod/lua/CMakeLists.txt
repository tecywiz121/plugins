project(MODLUA)

set(CMAKE_C_FLAGS "-g -Wall -Wextra -Werror -Wstrict-aliasing -std=c99 -pedantic")

add_library(mod_lua SHARED lua.c)
include_directories(${LUA_INCLUDE_DIR})
target_link_libraries(mod_lua ${LUA_LIBRARIES})

add_custom_command(TARGET mod_lua POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${MODLUA_SOURCE_DIR}/test.lua"
    $<TARGET_FILE_DIR:sharedobj>)
add_custom_command(TARGET mod_lua POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${MODLUA_SOURCE_DIR}/test_lua.mod"
    $<TARGET_FILE_DIR:sharedobj>)
