#include <iostream>
#include "cpu/cpu.h"
#include "memory/memory.h"

int main() {
    CPU cpu;
    Memory mem;

    // program
    mem.write8(0, 0xB8); // MOV
    mem.write32(1, 0x12345678);
    
    mem.write8(5, 0x05); // ADD
    mem.write32(6, 0x11111111);

    mem.write8(10, 0x2D); // SUB
    mem.write32(11, 0x11111111);

    mem.write8(15, 0x31); // XOR
    mem.write32(16, 0x12345679);

    mem.write8(20, 0xF4); // HLT

    // run
    cpu.execute(mem);

    // dump cpu
    std::cout << std::hex;
    std::cout << "EAX = " << cpu.eax << "\n";
    std::cout << "ZF = " << cpu.flags.ZF << "\n";
    std::cout << "SF = " << cpu.flags.SF << "\n";
    std::cout << "CF = " << cpu.flags.CF << "\n";
    std::cout << "OF = " << cpu.flags.OF << "\n";
}