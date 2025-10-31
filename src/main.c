#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gb/gb.h"
#include "disassemble.h"

d_opcodes *ops;

void print_status(gb_gameboy *gb) {
    printf("Cycles: %7lld | AF: %04X | A: %02X | F: %02X         \n", gb->cpu->cycles, gb->cpu->registers.af, gb->cpu->registers.a, gb->cpu->registers.f);
    printf("Opcode:      %02X | BC: %04X | B: %02X | C: %02X\n", gb->cpu->opcode, gb->cpu->registers.bc, gb->cpu->registers.b, gb->cpu->registers.c);
    printf("Addr. Bus: %04X | DE: %04X | D: %02X | E: %02X\n", gb->cpu->addr_bus, gb->cpu->registers.de, gb->cpu->registers.d, gb->cpu->registers.e);
    printf("Data Bus:    %02X | HL: %04X | H: %02X | L: %02X\n", gb->cpu->data_bus, gb->cpu->registers.hl, gb->cpu->registers.h, gb->cpu->registers.l);
    printf("mem[pc]:     %02X | SP: %04X | PC: %04X", gb_bus_read(gb->bus, gb->cpu->registers.pc), gb->cpu->registers.sp, gb->cpu->registers.pc);
}

void print_assembly(gb_gameboy *gb) {
    char instruction[50] = {};
    uint8_t op = 0;

    d_opcode *prefix;
    uint8_t offset = 0;

    if (gb->cpu->opcode != 0xcb) {
        prefix = ops->unprefixed;
        op = gb->cpu->opcode;
    }
    else {
        prefix = ops->cbprefixed;
        offset = 1;
        op = gb_bus_read(gb->bus, gb->cpu->registers.pc);
    }

    strcpy(instruction, prefix[op].mnemonic);

    if (prefix[op].operands_len > 0) {
        for (int i = 0; i < prefix[op].operands_len; i++) {
            strcat(instruction, " ");
            char x[10];

            if (!strcmp(prefix[op].operands[i].name, "n8")) {
                sprintf(x, "%02X", gb_bus_read(gb->bus, gb->cpu->registers.pc + offset));
                strcat(instruction, x);
            }
            else if (!strcmp(prefix[op].operands[i].name, "a8")) {
                sprintf(x, "[FF00+%02X]", gb_bus_read(gb->bus, gb->cpu->registers.pc + offset));
                strcat(instruction, x);
            }
            else if (!strcmp(prefix[op].operands[i].name, "e8")) {
                sprintf(x, "%d", (int8_t)gb_bus_read(gb->bus, gb->cpu->registers.pc + offset));
                strcat(instruction, x);
            }
            else if (!strcmp(prefix[gb->cpu->opcode].operands[i].name, "n16")) {
                sprintf(x, "%04X", gb_bus_read(gb->bus, gb->cpu->registers.pc + offset) | gb_bus_read(gb->bus, gb->cpu->registers.pc + 1 + offset) << 8);
                strcat(instruction, x);
            }
            else if (!strcmp(prefix[gb->cpu->opcode].operands[i].name, "a16")) {
                sprintf(x, "[%04X]", gb_bus_read(gb->bus, gb->cpu->registers.pc + offset) | gb_bus_read(gb->bus, gb->cpu->registers.pc + 1 + offset) << 8);
                strcat(instruction, x);
            }
            else {
                if (!prefix[gb->cpu->opcode].operands[i].immediate) {
                    sprintf(x, "[%s]", prefix[gb->cpu->opcode + offset].operands[i].name);
                    strcat(instruction, x);
                }
                else {
                    strcat(instruction, prefix[gb->cpu->opcode + offset].operands[i].name);
                }
            }
        }
    }
    printf("%s                 \n", instruction);
}

int main(void) {
    gb_gameboy *gb = gb_gameboy_init();

    ops = d_init("/Users/josem/Downloads/ops.json");

    gb_bus_load_bios(gb->bus, "/Users/josem/Downloads/dmg_boot.bin");
    gb_bus_attach_cart(gb->bus, "/Users/josem/Downloads/cpu_instrs.gb");

    gb->cpu->registers.a = 0x01;
    gb->cpu->registers.f = 0x80;
    gb->cpu->registers.b = 0x00;
    gb->cpu->registers.c = 0x13;
    gb->cpu->registers.d = 0x00;
    gb->cpu->registers.e = 0xd8;
    gb->cpu->registers.h = 0x01;
    gb->cpu->registers.l = 0x4d;
    gb->cpu->registers.pc = 0x0100;
    gb->cpu->registers.sp = 0xfffe;

    for (int i = 0; i < 100000000000; i++) {
        gb_cpu_clock(gb->cpu);

        if (gb->cpu->instruction_cycles == 0) {
            //printf("\x1b[6;0H");
            //print_assembly(gb);

            //printf("\x1b[0;0H");
            //print_status(gb);
        }
    }

    free(ops->json);
    free(ops);

    return 0;
}
