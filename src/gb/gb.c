#include "gb.h"
#include "cpu.h"
#include "io.h"

#include <stdlib.h>

gb_gameboy* gb_gameboy_init() {
    gb_gameboy *gb = malloc(sizeof(gb_gameboy));

    if (gb == nullptr) {
        return nullptr;
    }

    gb->bus = gb_bus_init();
    if (gb->bus == nullptr) {
        free(gb);
        return nullptr;
    }

    gb->cpu = gb_cpu_init();
    if (gb->cpu == nullptr) {
        gb_bus_destroy(gb->bus);
        free(gb);
        return nullptr;
    }

    gb->cpu->bus = gb->bus;
    gb->cpu->bus_read = &gb_bus_read;
    gb->cpu->bus_write = &gb_bus_write;

    gb->master_cycles = 0;

    return gb;
}

// clocks the master clock
void gb_gameboy_clock_master(gb_gameboy *gb) {
    // cpu clocked at GB_CLOCK_CPU hz
    if (gb->master_cycles % GB_CLOCK_CPU_DIV == 0) {
        gb_cpu_clock(gb->cpu);
    }

    // divider timer
    if (gb->master_cycles % GB_CLOCK_DIV_DIV == 0) {
        gb->bus->io[0xff00 ^ GB_IO_DIV]++;
    }

    // timer counter
    int divisions = 0;
    uint8_t tac = gb->bus->io[0xff00 ^ GB_IO_TAC];
    switch (tac & 0x3) {
        case 0:
            divisions = 1024;
            break;
        case 1:
            divisions = 16;
            break;
        case 2:
            divisions = 64;
            break;
        case 3:
        default:
            divisions = 256;
            break;
    }

    if (tac & 0x4) {
        if (gb->master_cycles % divisions == 0) {
            gb->bus->io[0xff00 ^ GB_IO_TIMA]++;

            if (gb->bus->io[0xff00 ^ GB_IO_TIMA] == 0x00) {
                gb->bus->io[0xff00 ^ GB_IO_TIMA] = gb->bus->io[0xff00 ^ GB_IO_TMA];
                gb->bus->io[0xff00 ^ GB_IO_IF] |= GB_CPU_INTERRUPT_TIMER;
            }
        }
    }

    gb->master_cycles++;
}

// clocks the master clock to the next cpu clock interval.
void gb_gameboy_clock_cpu(gb_gameboy *gb) {
    do {
        gb_gameboy_clock_master(gb);
    }
    while (gb->master_cycles % GB_CLOCK_CPU_DIV != 0);
}

void gb_gameboy_load_bootrom(gb_gameboy *gb, const char *bios) {
    gb_bus_load_bios(gb->bus, bios);
}

void gb_gameboy_load_rom(gb_gameboy *gb, const char *rom) {
    gb_bus_attach_cart(gb->bus, rom);
}

void gb_gameboy_destroy(gb_gameboy *gb) {
    gb_cpu_destroy(gb->cpu);
    gb_bus_destroy(gb->bus);
    free(gb);
}
