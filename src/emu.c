#include <stdio.h>
#include <stdint.h>
#include "emu.h"

Instruction byteToInstruction(uint8_t instByte) {
    Instruction inst = {0};

    if (instByte <= 0x0f) {
        inst.opcode = instByte;
        return inst;
    }

    inst.opcode = instByte > 0x80 ? instByte & 0xC0 : instByte & 0xF0;  // if the instruction is a jump cut away everything exept the upper to 2 bits. Else leave the upper 4 bits
    inst.data = instByte > 0x80 ? instByte & 0x3F : instByte & 0x0F;    // if the instruction is a jump get the address. if it is not get the lower 4 bits

    return inst;
}

int executeInstruction(Instruction inst, uint8_t* ram, uint8_t* pc, uint8_t* linkReg, uint8_t* sourceReg, uint8_t* destReg, uint8_t* carryFlag, uint8_t* isHalted) {
    (*pc)++;        // always increment pc. if we jump that overwrites it so its fine      
    *pc &= 0x3f;    // make sure pc does not get bigger than 63
    
    switch (inst.opcode) {
        case 0x10:  // add
            *destReg = *sourceReg + ram[inst.data];
            *carryFlag = *destReg >> 4;   // set the carry flag if the result is bigger than 15
            *destReg &= 0x0f;   // make sure the result is not bigger than 15
            break;

        case 0x20:  // sub
            *destReg = *sourceReg + ((~ram[inst.data])+1);  // subtraction
            *carryFlag = (*destReg >> 4) & 1;   // set the carry flag if the result is bigger than 15
            *destReg &= 0x0f;   // make sure the result is not bigger than 15
            break;

        case 0x30:  // xor
            *destReg = *sourceReg ^ ram[inst.data];
            *destReg &= 0x0f;
            break;

        case 0x40: // or
            *destReg = *sourceReg | ram[inst.data];
            *destReg &= 0x0f;
            break;

        case 0x50:  //  ldi
            *destReg = inst.data;
            break;
        
        case 0x60:  // mdr (move destReg ram)
            ram[inst.data] = *destReg;
            break;
        
        case 0x70:  // mrs  (move ram source)
            *sourceReg = ram[inst.data];
            break;

        case 0x80:  // jnc (jump not carry)
            *pc = *carryFlag ? *pc : inst.data; // if the carry flag is set don't jump. If it is set jump
            break;

        case 0xC0:  // call
            *linkReg = *pc; // pc is allready plus 1 so dont add even more
            *pc = inst.data;
            break;
        
        case 0x00:  // halt
            *isHalted = 1;
            break;

        case 0x01:  // ret
            *pc = *linkReg;
            break;

        case 0x02:  // mds (move destReg sourceReg)
            *sourceReg = *destReg;
            break;

        case 0x03:  // not
            *destReg = ~(*sourceReg);
            *destReg &= 0x0f;
            break;

        case 0x04: // rsh
            *destReg = *sourceReg>>1;
            *carryFlag = *sourceReg & 1;    // if the lsb is set before we shift. set the carry flag
            break;
        
        case 0x05: // lsh
            *destReg = *sourceReg<<1;
            *carryFlag = *destReg>>4;   // if the 5th bit is set, set the carry flag
            *destReg &= 0x0f;
            break;

        case 0x06: // rro
            *destReg = *sourceReg>>1;
            *destReg = *sourceReg & 1 ? *destReg|0b1000 : *destReg; // if the lsb is set, set the msb if not leave as is
            break;

        case 0x07: // lro
            *destReg = *sourceReg<<1;
            *destReg = *sourceReg>>4 ? *destReg|0b0001 : *destReg; // if the msb is set, set the lsb if not leave as is
            *destReg &= 0x0f;
            break;
        
        case 0x08:  // in
            printf("Input: ");
            scanf("%hhd", destReg);
            *destReg &= 0x0f;
            break;

        case 0x09:  // out
            printf("OUTPUT: %hhd\n", *destReg);
            break;
        
        case 0x0A:  // bell
            printf("\n\033[106mBELL\033[0m\n");
            break;

        default:
            printf("Error: unknown opcode %hhx\n", inst.opcode);
            return -1;
    }

    return 0;
}