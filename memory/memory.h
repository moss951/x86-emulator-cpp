#pragma once
#include <cstdint>
#include <stdexcept> 

class Memory {
public:
    static const size_t SIZE = 1024 * 1024; // 1MB
    uint8_t data[SIZE];

    Memory();

    uint8_t read8(uint32_t addr);
    uint32_t read32(uint32_t addr);

    void write8(uint32_t addr, uint8_t value);
    void write32(uint32_t addr, uint32_t value);
};