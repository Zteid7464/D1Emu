#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "emu.h"

int main(int argc, char* argv[]) {
    Instruction program[64] = {0};

    FILE* inputFile = NULL; 

    uint8_t ram[16] = {0};

    uint8_t pc = 0;
    uint8_t linkReg = 0;
    
    uint8_t sourceReg = 0;
    uint8_t destReg = 0;

    uint8_t carryFlag = 0;
    uint8_t isHalted = 0;

    if (argc < 2) {
        printf("You have to specifie the input file! Dumbass!\n");
        return -1;
    }

    if ((inputFile = fopen(argv[1], "rb")) == NULL) {
        printf("Could not open file %s\n", argv[1]);
        return -1;
    }

    uint8_t currentInstByte = 0;
    int i = 0;

    while (fread(&currentInstByte, 1, 1, inputFile) != 0 && i < 64) {
        program[i] = byteToInstruction(currentInstByte);
        i++;
    }


    // the main loop
    while (!isHalted) {
        if (executeInstruction(program[pc], ram, &pc, &linkReg, &sourceReg, &destReg, &carryFlag, &isHalted) != 0) {
            printf("invalid instruction at address %hhd\n", pc);
            return -1;
        }

        // printf("\nPC = %hhd CF = %hhd SR = %hhd DR = %hhd ISHALTED = %hhd\n", pc, carryFlag, sourceReg, destReg, isHalted);

        // for (i = 0; i < 16; i++) {
        //     printf(" Ram[%d] = %hhd ", i, ram[i]);
        // }

        // sleep(1);
    }


    return 0;
}