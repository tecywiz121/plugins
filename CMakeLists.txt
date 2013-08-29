cmake_minimum_required(VERSION 2.8)
project(SHAREDOBJ)

set(DYNCALL_DIR "lib/dyncall")

# Add custom cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SHAREDOBJ_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SHAREDOBJ_SOURCE_DIR}/${DYNCALL_DIR}/buildsys/cmake/Modules/")

# Build required in-tree libraries
add_subdirectory(${SHAREDOBJ_SOURCE_DIR}/lib/dyncall)

# Build the core application
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -Wstrict-aliasing -std=c++11 -pedantic")

include_directories(${SHAREDOBJ_SOURCE_DIR}/include)
add_executable(sharedobj src/loader.cpp src/plugin.cpp src/plugin_manager.cpp src/plugin_description.cpp src/plugin_library.cpp src/function.cpp src/invocation.cpp)

include_directories("${SHAREDOBJ_SOURCE_DIR}/${DYNCALL_DIR}/dynload")
include_directories("${SHAREDOBJ_SOURCE_DIR}/${DYNCALL_DIR}/dyncall")
target_link_libraries(sharedobj dynload_s dyncall_s ${CMAKE_DL_LIBS})

# Build optional modules
find_package(Lua51)

if (LUA51_FOUND)
    add_subdirectory(${SHAREDOBJ_SOURCE_DIR}/src/mod/lua)
endif (LUA51_FOUND)
