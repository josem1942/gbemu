#ifndef GB_H
#define GB_H

#define GB_MASTER_CLOCK 4194304

#include "cpu.h"
#include "bus.h"

typedef struct gb_gameboy {
    gb_cpu_core* cpu;
    gb_bus* bus;
} gb_gameboy;

gb_gameboy* gb_gameboy_init();

#endif //GB_H
