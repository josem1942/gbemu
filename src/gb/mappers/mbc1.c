#include "mbc1.h"

uint8_t gb_mapper_mbc1_read(gb_cart *cart, uint16_t address) {
    if (address < 0x4000) {
        return cart->rom[0][address];
    }
    if (address < 0x8000) {
        if (cart->rom_bank < cart->rom_len)
            return cart->rom[cart->rom_bank][address ^ 0x4000];
    }

    return 0xff;
}

void gb_mapper_mbc1_write(gb_cart *cart, uint16_t address, uint8_t data) {
    if (address >= 0x2000 && address < 0x4000) {
        uint8_t bank = data & 0b00011111;

        if (bank == 0) {
            cart->rom_bank = 0;
        }
        else {
            cart->rom_bank = (bank - 1) % cart->rom_len;
        }
    }
}
