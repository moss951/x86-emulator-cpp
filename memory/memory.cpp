#include "memory.h"

uint8_t Memory::read8(uint32_t addr) {
    if (addr >= SIZE) {
        throw std::out_of_range("Memory read8 out of bounds");
    }
    return data[addr];
}

uint32_t Memory::read32(uint32_t addr) {
    if (addr + 3 >= SIZE) {
        throw std::out_of_range("Memory read32 out of bounds");
    }

    // little endian: shift smaller, subsequent addresses left in a 32 bit int
    // bitwise OR on each byte to combine into final 32 bit value
    return  (uint32_t)data[addr] |
           ((uint32_t)data[addr + 1] << 8) |
           ((uint32_t)data[addr + 2] << 16) |
           ((uint32_t)data[addr + 3] << 24);
}

void Memory::write8(uint32_t addr, uint8_t val) {
    if (addr >= SIZE) {
        throw std::out_of_range("Memory write8 out of bounds");
    }
    data[addr] = val;
}

void Memory::write32(uint32_t addr, uint32_t val) {
    if (addr + 3 >= SIZE) {
        throw std::out_of_range("Memory write32 out of bounds");
    }

    // right shift target value to end of the 32 bits
    // bitwise AND 0xFF to truncate to lowest byte
    data[addr]     = val & 0xFF;
    data[addr + 1] = (val >> 8) & 0xFF;
    data[addr + 2] = (val >> 16) & 0xFF;
    data[addr + 3] = (val >> 24) & 0xFF;
}