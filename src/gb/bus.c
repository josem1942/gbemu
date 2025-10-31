#include "bus.h"

#include <stdio.h>
#include <stdlib.h>

gb_bus* gb_bus_init() {
    gb_bus *bus = malloc(sizeof(gb_bus));

    if (bus == nullptr) {
        return nullptr;
    }

    bus->cartridge = nullptr;

    bus->vram_bank = 0;
    bus->wram_bank = 0;
    bus->io[0x50] = 0;
    bus->ie = 0;

    return bus;
}

void gb_bus_destroy(gb_bus *bus) {
    if (bus != nullptr) {
        free(bus);
    }
}

void gb_bus_attach_cart(gb_bus *bus, const char *rom) {
    if (bus == nullptr) {
        return;
    }

    bus->cartridge = gb_cart_init();
    gb_cart_load(bus->cartridge, rom);
}

void gb_bus_load_bios(gb_bus *bus, const char *bios) {
    if (bus == nullptr) {
        return;
    }

    FILE *fp = fopen(bios, "rb");

    if (fp == nullptr) {
        return;
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) != 256) {
        return;
    }

    fseek(fp, 0, SEEK_SET);
    fread(bus->boot_rom, 1, 256, fp);
    fclose(fp);
}

void gb_bus_write(gb_bus *bus, uint16_t address, uint8_t data) {
    if (address == 0xffff) {
        bus->ie = data;
    }

    if (address < 0x8000) {
        if (bus->cartridge != nullptr) {
            bus->cartridge->cart_write(bus->cartridge, address, data);
        }
    }
    else if (address < 0xa000) {
        bus->vram[bus->vram_bank % 2][address ^ 0x8000] = data;
    }
    else if (address < 0xc000) {
        if (bus->cartridge != nullptr) {
            bus->cartridge->cart_write(bus->cartridge, address, data);
        }
    }
    else if (address < 0xd000) {
        bus->wram[0][address ^ 0xc000] = data;
    }
    else if (address < 0xe000) {
        bus->wram[bus->wram_bank % 8][address ^ 0xd000] = data;
    }
    else if (address < 0xfe00) {
        // TODO: properly do echo ram
    }
    else if (address < 0xfea0) {
        bus->oam[address ^ 0xfe00] = data;
    }
    else if (address < 0xff00) {
        // restricted
    }
    else if (address < 0xff80) {
        bus->io[address ^ 0xff00] = data;

        if (address == 0xff02) {
            if (data == 0x81) {
                printf("%c", (char)bus->io[0x01]);
                bus->io[address ^ 0xff00] &= ~0x80;
            }
        }
    }
    else if (address < 0xffff) {
        bus->hram[address ^ 0xff80] = data;
    }
}

uint8_t gb_bus_read(gb_bus *bus, uint16_t address) {
    if (address == 0xffff) {
        return bus->ie;
    }

    if (address < 0x8000) {
        if (address < 0x0100 && !bus->io[0x50] ) {
            return bus->boot_rom[address];
        }

        if (bus->cartridge != nullptr) {
            return bus->cartridge->cart_read(bus->cartridge, address);
        }
    }
    else if (address < 0xa000) {
        return bus->vram[bus->vram_bank % 2][address ^ 0x8000];
    }
    else if (address < 0xc000) {
        if (bus->cartridge != nullptr) {
            return bus->cartridge->cart_read(bus->cartridge, address);
        }
    }
    else if (address < 0xd000) {
        return bus->wram[0][address ^ 0xc000];
    }
    else if (address < 0xe000) {
        return bus->wram[bus->wram_bank % 8][address ^ 0xd000];
    }
    else if (address < 0xfe00) {
        return 0xff; // TODO: properly do echo ram
    }
    else if (address < 0xfea0) {
        return bus->oam[address ^ 0xfe00];
    }
    else if (address < 0xff00) {
        return 0xff; // restricted
    }
    else if (address < 0xff80) {
        return bus->io[address ^ 0xff00];
    }
    else if (address < 0xffff) {
        return bus->hram[address ^ 0xff80];
    }

    return 0xff;
}
