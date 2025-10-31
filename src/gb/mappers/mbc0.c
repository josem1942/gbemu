#include "mbc0.h"

uint8_t gb_mapper_mbc0_read(gb_cart *cart, uint16_t address) {
    if (address < 0x4000) {
        return cart->rom[0][address];
    }
    if (address < 0x8000) {
        return cart->rom[1][address ^ 0x4000];
    }

    return 0xff;
}

void gb_mapper_mbc0_write(gb_cart *cart, uint16_t address, uint8_t data) {

}
