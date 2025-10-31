#ifndef MBC0_H
#define MBC0_H

#include <stdint.h>

#include "../cartridge.h"

uint8_t gb_mapper_mbc0_read(gb_cart *cart, uint16_t address);
void gb_mapper_mbc0_write(gb_cart *cart, uint16_t address, uint8_t data);

#endif //MBC0_H
