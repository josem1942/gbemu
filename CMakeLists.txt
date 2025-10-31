cmake_minimum_required(VERSION 3.29)

set(VCPKG_ROOT "vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(gbemu C)

set(CMAKE_C_STANDARD 23)

find_package(SDL3 CONFIG REQUIRED)
find_package(cJSON CONFIG REQUIRED)

add_executable(gbemu src/main.c
        src/gb/gb.h
        src/gb/cpu.h
        src/gb/cpu.c
        src/gb/gb.c
        src/gb/bus.h
        src/gb/bus.c
        src/gb/cartridge.h
        src/gb/cartridge.c
        src/gb/mappers/mbc0.c
        src/gb/mappers/mbc0.h
        src/gb/mappers/mbc1.c
        src/gb/mappers/mbc1.h
        src/gb/io.h
        src/debug/disassembler.c
        src/debug/disassembler.h
        src/debug/debug.c
        src/debug/debug.h
)

target_link_libraries(gbemu PRIVATE SDL3::SDL3 cjson)
