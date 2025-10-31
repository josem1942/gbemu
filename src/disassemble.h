#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

typedef struct d_operand {
    char name[10];
    bool immediate;
} d_operand;

typedef struct d_opcode {
    char mnemonic[15];
    d_operand operands[2];
    int operands_len;
} d_opcode;

typedef struct d_opcodes {
    d_opcode unprefixed[256];
    d_opcode cbprefixed[256];
    char *json;
}  d_opcodes;

d_opcodes* d_init(const char *file);

#endif //DISASSEMBLE_H
