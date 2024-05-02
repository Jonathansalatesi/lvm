cmake_minimum_required(VERSION 3.27)
project(lvm)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(api)
add_subdirectory(vm)

add_executable(lvm main.cpp
        def.h
        binchunk.cpp
        create.cpp
        delete.cpp
        lua_type.h
        vm/vm.cpp
        vm/vm.h
        lua_stack.cpp
        lua_stack.h
        lua_state.cpp
        lua_state.h
        lua_math.cpp
        lua_math.h
)

# 添加链接库
target_link_libraries(lvm API)
target_link_libraries(lvm VM)
target_compile_options(lvm PUBLIC -O3 -Wall)