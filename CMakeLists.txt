cmake_minimum_required(VERSION 3.29)
project(gbemu C)

set(CMAKE_C_STANDARD 23)

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
        src/cjson/cjson.h
        src/cjson/cjson.c
        src/gb/io.h
        src/debug/disassembler.c
        src/debug/disassembler.h
        src/debug/debug.c
        src/debug/debug.h
)
