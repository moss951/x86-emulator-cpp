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

    mem.write8(20, 0x40); // INC
    mem.write8(21, 0x48); // DEC

    mem.write8(22, 0x3D); // CMP
    mem.write32(23, 0x02);

    mem.write8(27, 0x74); // JE/JNZ
    mem.write8(28, 0x01);

    mem.write8(29, 0x40); // INC should not be skipped

    mem.write8(30, 0x75); // JE/JZ
    mem.write8(31, 0x01);

    mem.write8(32, 0x40); // INC should be skipped

    mem.write8(33, 0xEB); // JMP
    mem.write8(34, 0x01);

    mem.write8(35, 0x40); // INC should be skipped
    mem.write8(36, 0x40); // JMP goes here

    mem.write8(37, 0x50); // PUSH 3

    mem.write8(38, 0xB8); // MOV
    mem.write32(39, 0x00);

    mem.write8(43, 0x58); // POP, eax should be 3 again

    mem.write8(44, 0xF4); // HLT

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