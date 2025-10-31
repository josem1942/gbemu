#ifndef DEBUG_H
#define DEBUG_H

#include "../gb/gb.h"
#include "disassembler.h"

void debug_print_registers(gb_gameboy *gb);
void debug_print_line(gb_gameboy *gb, disasm_opcodes *ops);
void debug_print_assembly(gb_gameboy *gb, disasm_opcodes *ops, char* buf);

#endif //DEBUG_H
