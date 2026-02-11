#pragma once
#include <cstdint>
#include <iostream>
#include <ios>
#include "../memory/memory.h"

class CPU {
public:
    uint32_t eax, ebx, ecx, edx; // general registers
    uint32_t esi, edi;           // source/destination indexes
    uint32_t ebp, esp;           // base/stack pointers
    uint32_t eip;                // instruction pointer

    // flag registers (zero, carry, sign, overflow)
    struct {
        bool ZF;
        bool CF;
        bool SF;
        bool OF;
    } flags;
    
    CPU();
    uint8_t fetch8(Memory& mem);
    uint32_t fetch32(Memory& mem);
    void execute(Memory& mem);
};