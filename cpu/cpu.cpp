#include "cpu.h"

CPU::CPU() {
    eax = ebx = ecx = edx = 0;
    esi = edi = 0;
    ebp = esp = 0;
    eip = 0;
    flags.ZF = flags.SF = flags.CF = flags.OF = 0;
}

uint8_t CPU::fetch8(Memory& mem) {
    return mem.read8(eip++);
}

uint32_t CPU::fetch32(Memory& mem) {
    uint32_t val = mem.read32(eip);
    eip += 4;
    return val;
}

void CPU::execute(Memory& mem) {
    bool running = true;

    while (running) {
        uint8_t opcode = fetch8(mem); // fetch 1 byte opcode

        switch (opcode) {
            case 0xB8: { // MOV EAX (4 bytes)
                // move value from instruction to eax register
                eax = fetch32(mem);
                break;
            }

            case 0x05: { // ADD EAX (4 bytes)
                uint32_t val = fetch32(mem);
                uint32_t result = eax + val;

                // update flags
                flags.ZF = (result == 0); // check if zero

                // check if result is negative (contains 1 on last bit)
                // stack flag will be 1 if negative, 0 if positive
                flags.SF = (result & 0x80000000) != 0;

                flags.CF = (result < eax); // checks if unsigned overflow happened 

                // checks if sign changed when performing operation
                // signed overflow happened if the first bit (sign) differs 
                flags.OF = ((eax ^ result) & (val ^ result) & 0x80000000) != 0;

                eax = result;
                break;
            }

            case 0xF4: { // HLT
                running = false;
                break;
            }

            default: {
                std::cerr << "Unknown opcode: " << std::hex << (int)opcode << "\n";
                running = false;
            }
        }
    }
}