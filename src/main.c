#include <stdio.h>

#include "gb/gb.h"
#include "debug/disassembler.h"
#include "debug/debug.h"

#ifdef _WIN32
const char JSON_OPCODES[] = "..\\..\\..\\..\\Downloads\\ops.json";
const char ROM[] = "..\\..\\..\\..\\Downloads\\instr_timing.gb";
const char BIOS[] = "E:\\bios\\gb\\dmg_boot.bin";
#else
const char JSON_OPCODES[] = "../../../Downloads/ops.json";
const char ROM[] = "../../../Downloads/cpu_instrs.gb";
const char BIOS[] = "../../../Downloads/dmg_boot.bin";
#endif

int main(void) {
    gb_gameboy *gb = gb_gameboy_init();
    disasm_opcodes *ops = disasm_init(JSON_OPCODES);

    gb_bus_attach_cart(gb->bus, ROM);
    gb_cpu_skip_bootrom(gb->cpu);

    char assembly[50] = {};

    while (1) {
        gb_gameboy_clock_cpu(gb);

        if (gb->cpu->instruction_cycles == 0) {
            //debug_print_line(gb, ops);
        }

        /*
         if (true) {
            printf("\x1b[0;0H");
            debug_print_registers(gb);

            if (gb->cpu->instruction_cycles == 0) {
                debug_print_assembly(gb, ops, assembly);
                printf("\x1b[6;0H");
                printf("%s              ", assembly);
            }
        }
        */
    }

    disasm_destroy(ops);
    gb_gameboy_destroy(gb);

    return 0;
}
