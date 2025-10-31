// parse opcode table from pandocs

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

typedef struct disasm_operand {
    char name[10];
    bool immediate;
} disasm_operand;

typedef struct disasm_opcode {
    char mnemonic[15];
    disasm_operand operands[2];
    int operands_len;
} disasm_opcode;

// root json structure
typedef struct disasm_opcodes {
    disasm_opcode unprefixed[256];
    disasm_opcode cbprefixed[256];
}  disasm_opcodes;

// initialize opcodes
disasm_opcodes* disasm_init(const char *file);

// destroy object
void disasm_destroy(disasm_opcodes *ops);

#endif //DISASSEMBLER_H