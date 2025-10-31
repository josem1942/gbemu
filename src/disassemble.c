#include "disassemble.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson.h"

d_opcodes* d_init(const char *file) {
    d_opcodes *opcodes = malloc(sizeof(d_opcodes));
    FILE *fp = fopen(file, "rb");
    if (fp == nullptr) {
        return nullptr;
    }
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    opcodes->json = malloc(len + 1);
    fseek(fp, 0, SEEK_SET);
    opcodes->json[len] = '\0';
    fread(opcodes->json, 1, len, fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(opcodes->json);

    if (json == nullptr) {
        free(opcodes->json);
        free(opcodes);
        return nullptr;
    }

    const cJSON *unprefixed = cJSON_GetObjectItemCaseSensitive(json, "unprefixed");

    for (int i = 0; i < 256; i++) {
        char hex[10];
        sprintf(hex, "0x%02X", i);

        const cJSON *opcode = cJSON_GetObjectItemCaseSensitive(unprefixed, hex);

        const cJSON *mnemonic = cJSON_GetObjectItemCaseSensitive(opcode, "mnemonic");
        strcpy(opcodes->unprefixed[i].mnemonic, mnemonic->valuestring);

        const cJSON *operands = cJSON_GetObjectItemCaseSensitive(opcode, "operands");
        const cJSON *operand = nullptr;

        cJSON_ArrayForEach(operand, operands) {
            const cJSON *name = cJSON_GetObjectItemCaseSensitive(operand, "name");
            const cJSON *imm = cJSON_GetObjectItemCaseSensitive(operand, "immediate");

            opcodes->unprefixed[i].operands[opcodes->unprefixed[i].operands_len].immediate = imm->valueint;
            strcpy(opcodes->unprefixed[i].operands[opcodes->unprefixed[i].operands_len].name, name->valuestring);

            const cJSON *inc = cJSON_GetObjectItemCaseSensitive(operand, "increment");
            if (inc != nullptr) {
                strcat(opcodes->unprefixed[i].operands[opcodes->unprefixed[i].operands_len].name, "+");
            }

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

        const cJSON *opcode = cJSON_GetObjectItemCaseSensitive(cbprefixed, hex);

        const cJSON *mnemonic = cJSON_GetObjectItemCaseSensitive(opcode, "mnemonic");
        strcpy(opcodes->cbprefixed[i].mnemonic, mnemonic->valuestring);

        const cJSON *operands = cJSON_GetObjectItemCaseSensitive(opcode, "operands");
        const cJSON *operand = nullptr;

        cJSON_ArrayForEach(operand, operands) {
            const cJSON *name = cJSON_GetObjectItemCaseSensitive(operand, "name");
            const cJSON *imm = cJSON_GetObjectItemCaseSensitive(operand, "immediate");

            opcodes->cbprefixed[i].operands[opcodes->cbprefixed[i].operands_len].immediate = imm->valueint;
            strcpy(opcodes->cbprefixed[i].operands[opcodes->cbprefixed[i].operands_len].name, name->valuestring);

            opcodes->cbprefixed[i].operands_len++;
        }
    }

    cJSON_Delete(json);

    return opcodes;
}
