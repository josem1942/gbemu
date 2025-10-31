#ifndef GBEMU_MBC1_H
#define GBEMU_MBC1_H

#include <stdint.h>

#include "../cartridge.h"

uint8_t gb_mapper_mbc1_read(gb_cart *cart, uint16_t address);
void gb_mapper_mbc1_write(gb_cart *cart, uint16_t address, uint8_t data);

#endif //GBEMU_MBC1_H