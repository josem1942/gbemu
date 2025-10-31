#include "gb.h"
#include "cpu.h"

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

    return gb;
}

void gb_gameboy_destroy() {

}
