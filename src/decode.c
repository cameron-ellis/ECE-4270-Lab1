#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//converts an 8 digit hex number to binary,
//and splits it unto RISC-V instruction format data


void decimalToBinary(int num, int * binary) {   
   for(int i = 0; i < 32; i++){
      binary[31-i] = num % 2;
      num /= 2;
   }
   for(int i = 0; i < 32; i++){
      printf("%d", binary[i]);
   }
}

int arrayCmp(int * ar




int main(){
   uint32_t instruction=0x01EF8B33;
   int binary[32];
   decimalToBinary(instruction, binary);
   int opcode[7];
   for(int i = 0; i < 7; i++){
     opcode[i] = binary[i + 25];
    }
   int rd[5];
   for(int i = 0; i < 5; i++){
     rd[5] = binary[20 + i];
   }
   int f3[3];
   for(int i = 0; i < 3; i++){
     f3 = binary[17 + i];
   }
   int rs1[5];
   for(int i = 0; i < 5; i++){
     rs1[5] = binary[12 + i];
   }
   int rs2[5];
   for(int i = 0; i < 5; i++){
     rs2[5] = binary[7 + i];
   }
   int f7[7];
   for(int i = 0; i < 7; i++){
     f7[i] = binary[i];
   }


}