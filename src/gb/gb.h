//  main gameboy object. handles linking everything together, along with this being the source of the clocks.

#ifndef GB_H
#define GB_H

#include "cpu.h"
#include "bus.h"

// Master clock: 4194304 hz or 8388608 hz for CGB
// CPU clock: 1048576 hz (1/4)
// Clock 0: 4096 hz (1/1024)
// Clock 1: 262144 hz (1/16)
// Clock 2: 65536 hz (1/64)
// Clock 3: 16384 hz (1/256)
// PPU : 1 dot = 4194304 hz
// dots remain the same regardless of cpu mode
#define GB_CLOCK_MASTER 4194304
#define GB_CLOCK_CPU 1048576
#define GB_CLOCK_CPU_DIV 4
#define GB_CLOCK_DIV_DIV 256

typedef struct gb_gameboy {
    gb_cpu_core* cpu;
    gb_bus* bus;

    uint64_t master_cycles;
} gb_gameboy;

// initialize the gameboy
gb_gameboy* gb_gameboy_init();

// clocks the master clock
void gb_gameboy_clock_master(gb_gameboy *gb);

// clocks the master clock to the next cpu clock interval.
void gb_gameboy_clock_cpu(gb_gameboy *gb);

// load bootrom, just calls the bus func
void gb_gameboy_load_bootrom(gb_gameboy *gb, const char *bios);

// load rom, just calls bus func
void gb_gameboy_load_rom(gb_gameboy *gb, const char *rom);

// free everything
void gb_gameboy_destroy(gb_gameboy *gb);

#endif //GB_H
