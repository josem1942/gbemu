#include "cartridge.h"

#include "mappers/mbc0.h"
#include "mappers/mbc1.h"

#include <stdio.h>
#include <stdlib.h>

gb_cart* gb_cart_init() {
    gb_cart *cart = malloc(sizeof(gb_cart));

    cart->ram_bank = 0;
    cart->ram_enabled = false;
    cart->rom_bank = 0;
    cart->mbc = 0;
    cart->ram_len = 0;
    cart->rom_len = 0;
    cart->cart_read = nullptr;
    cart->cart_write = nullptr;

    return cart;
}

void gb_cart_load(gb_cart *cart, const char *file) {
    // load main rom
    FILE *fp = fopen(file, "rb");

    if (fp == nullptr) {
        return;
    }

    fseek(fp, 0, SEEK_END);
    const size_t len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    size_t banks = len / 0x4000;
    if (len % 0x4000) {
        banks++;
    }

    cart->rom = malloc(banks * sizeof(uint8_t*));
    if (cart->rom == nullptr) {
        return;
    }

    for (int i = 0; i < banks; i++) {
        cart->rom[i] = malloc(0x4000);

        if (cart->rom[i] == nullptr) {
            return;
        }
    }

    for (int i = 0; i < len; i += 0x4000) {
        fread(cart->rom[i / 0x4000], 1, 0x4000, fp);
    }

    fclose(fp);

    cart->rom_len = banks;

    // check mbc type
    switch (cart->rom[0][0x147]) {
        case 0x00:
            cart->cart_read = &gb_mapper_mbc0_read;
            cart->cart_write = &gb_mapper_mbc0_write;
            break;
        case 0x01:
            cart->cart_read = &gb_mapper_mbc1_read;
            cart->cart_write = &gb_mapper_mbc1_write;
            break;
        case 0x02:
        case 0x03:
            if (cart->rom_len >= 32) {
                cart->ram = malloc(4 * sizeof(uint8_t*));
                if (cart->ram == nullptr) {
                    return;
                }

                for (int i = 0; i < 4; i++) {
                    cart->ram[i] = malloc(0x2000);

                    if (cart->ram[i] == nullptr) {
                        return;
                    }
                }
                cart->ram_len = 4;
            }
            else {
                cart->ram = malloc(1 * sizeof(uint8_t*));
                if (cart->ram == nullptr) {
                    return;
                }

                for (int i = 0; i < 1; i++) {
                    cart->ram[i] = malloc(0x2000);

                    if (cart->ram[i] == nullptr) {
                        return;
                    }
                }
                cart->ram_len = 1;
            }
            cart->cart_read = &gb_mapper_mbc1_read;
            cart->cart_write = &gb_mapper_mbc1_write;
            break;
        case 0x08:
            cart->ram = malloc(1 * sizeof(uint8_t*));
            if (cart->ram == nullptr) {
                return;
            }

            for (int i = 0; i < 1; i++) {
                cart->ram[i] = malloc(0x2000);

                if (cart->ram[i] == nullptr) {
                    return;
                }
            }
            cart->ram_len = 1;
            cart->cart_read = &gb_mapper_mbc1_read;
            cart->cart_write = &gb_mapper_mbc1_write;
            break;
    }
}

void gb_cart_destroy(gb_cart *cart) {
    if (cart != nullptr) {

    }
}
