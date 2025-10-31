#ifndef BUS_H
#define BUS_H

#include <stdint.h>

#include "cartridge.h"

typedef struct gb_bus {
    uint8_t boot_rom[0x100];
    gb_cart *cartridge; // cartridge and external ram

    uint8_t vram[2][0x2000]; // bankable vram
    uint8_t vram_bank; // if not set, bank 0, else bank 1

    uint8_t wram[8][0x1000]; // bankable wram
    uint8_t wram_bank;

    uint8_t oam[0xA0];

    uint8_t io[0x80];
    uint8_t hram[0x80];

    uint8_t ie;

} gb_bus;

gb_bus* gb_bus_init();
void gb_bus_destroy(gb_bus *bus);

void gb_bus_attach_cart(gb_bus *bus, const char *rom);
void gb_bus_load_bios(gb_bus *bus, const char *bios);

void gb_bus_write(gb_bus *bus, uint16_t address, uint8_t data);
uint8_t gb_bus_read(gb_bus *bus, uint16_t address);

#endif //BUS_H

/*

uint8_t file[];
size_t bank_offset;
uint8_t ram[];

*/
