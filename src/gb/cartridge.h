#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct gb_cart gb_cart;

struct gb_cart {
    uint8_t **rom;
    uint8_t **ram;

    size_t rom_len;
    size_t ram_len;

    uint8_t mbc;
    uint8_t rom_bank;
    uint8_t ram_bank;

    bool ram_enabled;

    uint8_t (*cart_read)(gb_cart *cart, uint16_t address);
    void (*cart_write)(gb_cart *cart, uint16_t address, uint8_t data);
};

gb_cart* gb_cart_init();
void gb_cart_load(gb_cart *cart, const char *file);

#endif //CARTRIDGE_H
