#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>
#include <stdlib.h>

typedef struct gb_cart gb_cart;

struct gb_cart {
    uint8_t **rom; // rom banks
    uint8_t **ram; // ram banks

    size_t rom_len; // rom banks length
    size_t ram_len; // ram banks length

    uint8_t mbc; // mbc value from cart header
    uint8_t rom_bank; // selected rom bank, zero indexed. (ie. if rom_bank is 0, then bank 1 is loaded)
    uint8_t ram_bank; // selected ram bank

    bool ram_enabled; // is ram enabled? for supported mbcs

    // mbc specific r/w
    uint8_t (*cart_read)(gb_cart *cart, uint16_t address);
    void (*cart_write)(gb_cart *cart, uint16_t address, uint8_t data);
};

gb_cart* gb_cart_init();
void gb_cart_load(gb_cart *cart, const char *file);
void gb_cart_destroy(gb_cart *cart);

#endif //CARTRIDGE_H
