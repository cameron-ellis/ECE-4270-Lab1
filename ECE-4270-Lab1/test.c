#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){
   uint32_t ins=0x01EF8B33;
   uint32_t opcode = (ins << 25) >> 25;
   uint32_t rd = (ins << 20) >> 27;
   uint32_t f3 = (ins << 17) >> 29;
   uint32_t rs1 = (ins << 12) >> 27;
   uint32_t rs2 = (ins << 7) >> 27;
   uint32_t f7 = ins >> 25;
   printf("opcode:%x \nrd: %x\nf3: %x\nrs1: %x\nrs2: %x\nf7: %x\n", opcode, rd, f3, rs1, rs2, f7);
}

/*typedef struct Instruction_Struct {
	char * instructionName;
	int rd, rs1, rs2;
} Translated_Instruction;*/