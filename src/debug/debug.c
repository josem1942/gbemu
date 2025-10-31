#include "debug.h"

#include "../gb/gb.h"

#include <stdio.h>
#include <string.h>

void debug_print_registers(gb_gameboy *gb) {
    printf("Cycles: %7lld | AF: %04X | A: %02X | F: %02X         \n", gb->cpu->cpu_cycles, gb->cpu->registers.af, gb->cpu->registers.a, gb->cpu->registers.f);
    printf("Master: %7lld | BC: %04X | B: %02X | C: %02X\n", gb->master_cycles, gb->cpu->registers.bc, gb->cpu->registers.b, gb->cpu->registers.c);
    printf("Opcode:      %02X | DE: %04X | D: %02X | E: %02X\n", gb->cpu->opcode, gb->cpu->registers.de, gb->cpu->registers.d, gb->cpu->registers.e);
    printf("                | HL: %04X | H: %02X | L: %02X\n", gb->cpu->registers.hl, gb->cpu->registers.h, gb->cpu->registers.l);
    printf("mem[pc]:     %02X | SP: %04X | PC: %04X", gb_bus_read(gb->bus, gb->cpu->registers.pc), gb->cpu->registers.sp, gb->cpu->registers.pc);
}

void debug_print_line(gb_gameboy *gb, disasm_opcodes *ops) {
    char assembly[50] = {};
    debug_print_assembly(gb, ops, assembly);
    printf("Master: %7lld | CPU Cycles: %7lld | AF: $%04X | BC: $%04X | DE: $%04X | HL: $%04X | SP: $%04X | PC: $%04X | OP: $%02X | %s\n",
        gb->master_cycles, gb->cpu->cpu_cycles, gb->cpu->registers.af, gb->cpu->registers.bc, gb->cpu->registers.de, gb->cpu->registers.hl, gb->cpu->registers.sp, gb->cpu->registers.pc, gb->cpu->opcode, assembly);
}

void debug_print_assembly(gb_gameboy *gb, disasm_opcodes *ops, char* buf) {
    // final instruction
    char instruction[50] = {};

    // final opcode
    uint8_t op = 0;

    disasm_opcode *prefix;
    uint8_t offset = 0;

    if (gb->cpu->opcode != 0xcb) {
        prefix = ops->unprefixed;
        op = gb->cpu->opcode;
    }
    else {
        // its a cb prefix
        prefix = ops->cbprefixed;
        offset = 1;
        op = gb_bus_read(gb->bus, gb->cpu->registers.pc);
    }

    // copy the name
    sprintf(instruction, "%s ", prefix[op].mnemonic);

    // go through each operand
    for (int i = 0; i < prefix[op].operands_len; i++) {
        char operand[15] = {};
        char final_operand[15] = {};

        if (!strcmp(prefix[op].operands[i].name, "n8")) {
            sprintf(operand, "$%02X", gb_bus_read(gb->bus, gb->cpu->registers.pc + offset));
        }
        else if (!strcmp(prefix[op].operands[i].name, "a8")) {
            sprintf(operand, "$FF00+%02X", gb_bus_read(gb->bus, gb->cpu->registers.pc + offset));
        }
        else if (!strcmp(prefix[op].operands[i].name, "e8")) {
            sprintf(operand, "%d", (int8_t)gb_bus_read(gb->bus, gb->cpu->registers.pc + offset));
        }
        else if (!strcmp(prefix[gb->cpu->opcode].operands[i].name, "n16")) {
            sprintf(operand, "$%04X", gb_bus_read(gb->bus, gb->cpu->registers.pc + offset) | gb_bus_read(gb->bus, gb->cpu->registers.pc + 1 + offset) << 8);
        }
        else if (!strcmp(prefix[gb->cpu->opcode].operands[i].name, "a16")) {
            sprintf(operand, "$%04X", gb_bus_read(gb->bus, gb->cpu->registers.pc + offset) | gb_bus_read(gb->bus, gb->cpu->registers.pc + 1 + offset) << 8);
        }
        else {
            strcpy(operand, prefix[gb->cpu->opcode + offset].operands[i].name);
        }

        if (!prefix[op].operands[i].immediate) {
            sprintf(final_operand, "[%s],", operand);
        }
        else {
            sprintf(final_operand, "%s,", operand);
        }

        strcat(instruction, final_operand);
    }

    // the last character is either a comma or a space
    instruction[strlen(instruction) - 1] = '\0';

    strcpy(buf, instruction);
}
