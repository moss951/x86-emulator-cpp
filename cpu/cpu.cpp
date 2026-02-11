#include "cpu.h"

CPU::CPU() {
    eax = ebx = ecx = edx = 0;
    esi = edi = 0;
    ebp = esp = Memory::SIZE;
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
            case 0xB8: { // MOV EAX (4 bytes immediate)
                // move value from instruction to eax register
                eax = fetch32(mem);
                break;
            }

            case 0x05: { // ADD EAX (4 bytes immediate)
                uint32_t val = fetch32(mem);
                uint32_t result = eax + val;

                // update flags

                // check if zero
                flags.ZF = (result == 0);

                // check if result is negative (contains 1 on last bit)
                // stack flag will be 1 if negative, 0 if positive
                flags.SF = (result & 0x80000000) != 0;

                // checks if unsigned overflow happened 
                flags.CF = (result < eax);

                // checks if sign changed when performing operation
                // signed overflow happened if the first bit (sign) differs 
                flags.OF = ((eax ^ result) & (val ^ result) & 0x80000000) != 0;

                eax = result;
                break;
            }

            case 0x2D: { // SUB EAX (4 bytes immediate)
                uint32_t val = fetch32(mem);
                uint32_t result = eax - val;

                flags.ZF = (result == 0);
                flags.SF = (result & 0x80000000) != 0;
                flags.CF = (val > eax);
                flags.OF = ((eax ^ result) & (val ^ result) & 0x80000000) != 0;

                eax = result;
                break;
            }

            case 0x40: { // INC EAX
                uint32_t result = eax + 1;
                flags.ZF = (result == 0);
                flags.SF = (result & 0x80000000) != 0;
                flags.OF = (result == 0x80000000) != 0; // max signed positive int + 1
                // CF not affected
                eax = result;
                break;
            }

            case 0x48: { // DEC EAX
                uint32_t result = eax - 1;
                flags.ZF = (result == 0);
                flags.SF = (result & 0x80000000) != 0;
                flags.OF = (result == 0x7FFFFFFF) != 0; // max signed positive int - 1
                eax = result;
                break;
            }

            case 0x25: { // AND EAX
                uint32_t val = fetch32(mem);
                uint32_t result = eax & val;

                flags.ZF = (result == 0);
                flags.SF = (result & 0x80000000) != 0;
                flags.CF = flags.OF = 0;

                eax = result;
                break;
            }

            case 0x0D: { // OR EAX
                uint32_t val = fetch32(mem);
                uint32_t result = eax | val;

                flags.ZF = (result == 0);
                flags.SF = (result & 0x80000000) != 0;
                flags.CF = flags.OF = 0;

                eax = result;
                break;
            }

            case 0x31: { // XOR EAX (4 bytes immediate)
                uint32_t val = fetch32(mem);
                uint32_t result = eax ^ val;

                flags.ZF = (result == 0);
                flags.SF = (result & 0x80000000) != 0;
                flags.CF = flags.OF = 0;

                eax = result;
                break;
            }

            case 0x3D: { // CMP EAX
                uint32_t val = fetch32(mem);
                uint32_t result = eax - val;
                flags.ZF = (result == 0);
                flags.SF = (result & 0x80000000) != 0;
                flags.CF = (val > eax);
                flags.OF = ((eax ^ val) & (eax ^ result) & 0x80000000) != 0;
                break;
            }

            case 0x74: { // JE/JNZ
                int8_t offset = fetch8(mem); // signed for jumping back
                if (flags.ZF) {
                    eip += offset; // offset relative to pointer
                }
                break;
            }

            case 0x75: { // JE/JZ (1 byte immediate)
                int8_t offset = fetch8(mem);
                if (!flags.ZF) {
                    eip += offset;
                }
                break;
            }

            case 0xEB: { // JMP (1 byte immediate)
                int8_t offset = fetch8(mem);
                eip += offset;
                break;
            }

            case 0x50: { // PUSH EAX
                esp -= 4; // 4 byte decrease
                mem.write32(esp, eax); // save eax to esp
                break;
            }

            case 0x58: { // POP EAX
                eax = mem.read32(esp); // load previous eax state back
                esp += 4; // 4 byte increase
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