#ifndef uint8_t
    #include <stdint.h>
#endif

typedef struct {
    uint8_t opcode;
    uint8_t data;
} Instruction;

Instruction byteToInstruction(uint8_t instByte);
int executeInstruction(Instruction inst, uint8_t* ram, uint8_t* pc, uint8_t* linkReg, uint8_t* sourceReg, uint8_t* destReg, uint8_t* carryFlag, uint8_t* isHalted);