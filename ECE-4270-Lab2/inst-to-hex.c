typedef struct r_type{
    char inst_name[7];
    int opcode, f3, f7, rd, rs1, rs2;
} r_type;

typedef struct i_type{
    char inst_name[7];
    int opcode, f3, imm, rd, rs1;
} i_type;

typedef struct s_type{
    char inst_name[7];
    int opcode, f3, imm, rd, rs1;
} s_type;

typedef struct b_type{
    char inst_name[7];
    int opcode, f3, imm, rs2, rs1;
} b_type;

typedef struct j_type{
    char inst_name[7];
    int opcode, imm, rd;
} j_type;


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char byteToHex(uint8_t byte);

void RtypeToHex(r_type instStruct, char * instruction){
    uint8_t byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, temp;
    uint8_t temp1, temp2;

    temp1 = instStruct.opcode << 4;
    byte0 = temp1 >> 4;

    temp1 = instStruct.rd << 7;
    temp1 = temp1 >> 4;
    temp2 = instStruct.opcode >> 4;
    byte1 = temp1 + temp2;

    byte2 = instStruct.rd >> 1;

    temp1 = instStruct.rs1 << 7;
    temp1 = temp1 >> 4;
    byte3 = instStruct.f3 + temp1;

    byte4 = instStruct.rs1 >>1;

    temp1 = instStruct.rs2 << 4;
    byte5 = temp1 >> 4;

    temp1 = instStruct.f7 << 4;
    temp1 = temp1 >> 4;
    temp2 = instStruct.rs2 >> 4;
    byte6 = temp1 + temp2;
    
    byte7 = instStruct.f7 >> 3;

    instruction[7] = byteToHex(byte0);
    instruction[6] = byteToHex(byte1);
    instruction[5] = byteToHex(byte2);
    instruction[4] = byteToHex(byte3);
    instruction[3] = byteToHex(byte4);
    instruction[2] = byteToHex(byte5);
    instruction[1] = byteToHex(byte6);
    instruction[0] = byteToHex(byte7);
    instruction[8] = '\0';
    //uint32_t instruction = (byte7 << 28) + (byte6 << 24) + (byte5 << 20) + (byte4 << 16) + (byte3 << 12) + (byte2 << 8) + (byte1 << 4) + byte0;
}

char byteToHex(uint8_t byte){
    switch(byte){
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
        default:
            return '0';
    }
}

int main(){
    r_type instruction;
    instruction.opcode = 0b0110011;
    instruction.f3 = 0b000;
    instruction.f7 = 0b0000000;
    instruction.rd = 13;
    instruction.rs1 = 13;
    instruction.rs2 = 8;
    char result[9];
    RtypeToHex(instruction, result);
    printf("Instruction = %s\n", result);
}