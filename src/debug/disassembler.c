#include "disassembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cjson/cjson.h"

disasm_opcodes* disasm_init(const char *file) {
    // open file
    disasm_opcodes *opcodes = malloc(sizeof(disasm_opcodes));
    if (opcodes == nullptr) {
        return nullptr;
    }

    memset(opcodes, 0, sizeof(disasm_opcodes));

    FILE *fp = fopen(file, "rb");
    if (fp == nullptr) {
        return nullptr;
    }

    // get length, and allocate memory for it
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    char *json_file = malloc(len + 1);

    // read the file
    fseek(fp, 0, SEEK_SET);
    json_file[len] = '\0';
    fread(json_file, 1, len, fp);
    fclose(fp);

    // open parser
    cJSON *json = cJSON_Parse(json_file);

    if (json == nullptr) {
        free(json_file);
        free(opcodes);
        return nullptr;
    }

    // start with unprefixed opcodes
    const cJSON *unprefixed = cJSON_GetObjectItemCaseSensitive(json, "unprefixed");
    for (int i = 0; i < 256; i++) {
        // convert index to a string in hex
        char hex[10];
        sprintf(hex, "0x%02X", i);

        // unprefixed->[op byte]
        const cJSON *opcode = cJSON_GetObjectItemCaseSensitive(unprefixed, hex);

        // unprefixed->[op byte]->mnemonic
        const cJSON *mnemonic = cJSON_GetObjectItemCaseSensitive(opcode, "mnemonic");
        strcpy(opcodes->unprefixed[i].mnemonic, mnemonic->valuestring);

        // unprefixed->[op byte]->operands
        const cJSON *operands = cJSON_GetObjectItemCaseSensitive(opcode, "operands");
        const cJSON *operand = nullptr;

        cJSON_ArrayForEach(operand, operands) {
            // unprefixed->[op byte]->operands[]->name
            const cJSON *name = cJSON_GetObjectItemCaseSensitive(operand, "name");

            // unprefixed->[op byte]->operands[]->immediate
            const cJSON *imm = cJSON_GetObjectItemCaseSensitive(operand, "immediate");

            opcodes->unprefixed[i].operands[opcodes->unprefixed[i].operands_len].immediate = imm->valueint;
            strcpy(opcodes->unprefixed[i].operands[opcodes->unprefixed[i].operands_len].name, name->valuestring);

            // unprefixed->[op byte]->operands[]->increment
            const cJSON *inc = cJSON_GetObjectItemCaseSensitive(operand, "increment");
            if (inc != nullptr) {
                strcat(opcodes->unprefixed[i].operands[opcodes->unprefixed[i].operands_len].name, "+");
            }

            // unprefixed->[op byte]->operands[]->decrement
            const cJSON *dec = cJSON_GetObjectItemCaseSensitive(operand, "decrement");
            if (dec != nullptr) {
                strcat(opcodes->unprefixed[i].operands[opcodes->unprefixed[i].operands_len].name, "-");
            }

            opcodes->unprefixed[i].operands_len++;
        }
    }

    const cJSON *cbprefixed = cJSON_GetObjectItemCaseSensitive(json, "cbprefixed");
    for (int i = 0; i < 256; i++) {
        char hex[10];
        sprintf(hex, "0x%02X", i);

        // cbprefixed->[op byte]
        const cJSON *opcode = cJSON_GetObjectItemCaseSensitive(cbprefixed, hex);

        // cbprefixed->[op byte]->mnemonic
        const cJSON *mnemonic = cJSON_GetObjectItemCaseSensitive(opcode, "mnemonic");
        strcpy(opcodes->cbprefixed[i].mnemonic, mnemonic->valuestring);

        // cbprefixed->[op byte]->operands
        const cJSON *operands = cJSON_GetObjectItemCaseSensitive(opcode, "operands");
        const cJSON *operand = nullptr;

        cJSON_ArrayForEach(operand, operands) {
            // cbprefixed->[op byte]->operands[]->name
            const cJSON *name = cJSON_GetObjectItemCaseSensitive(operand, "name");

            // cbprefixed->[op byte]->operands[]->immediate
            const cJSON *imm = cJSON_GetObjectItemCaseSensitive(operand, "immediate");

            opcodes->cbprefixed[i].operands[opcodes->cbprefixed[i].operands_len].immediate = imm->valueint;
            strcpy(opcodes->cbprefixed[i].operands[opcodes->cbprefixed[i].operands_len].name, name->valuestring);

            opcodes->cbprefixed[i].operands_len++;
        }
    }

    cJSON_Delete(json);
    free(json_file);

    return opcodes;
}

void disasm_destroy(disasm_opcodes *ops) {
    if (ops != nullptr) {
        free(ops);
    }
}
