#include "riscv-inst-read.h"

struct inst inst_read(const char* filepath) {
    inst result_inst;
    char inst_name[7];
    char trash[10];

    FILE *f = fopen(filepath, "r");

    fscanf(f, "%s ", inst_name);

    switch (inst_name) {
        case "add" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 0;
            inst_info.f7 = 0;
            result_inst.type_block = &inst_info;
            break;
        case "sub" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 0;
            inst_info.f7 = 32;
            result_inst.type_block = &inst_info;
            break;
        case "xor" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 4;
            inst_info.f7 = 0;
            result_inst.type_block = &inst_info;
            break;
        case "or" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 6;
            inst_info.f7 = 0;
            result_inst.type_block = &inst_info;
            break;
        case "and" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 7;
            inst_info.f7 = 0;
            result_inst.type_block = &inst_info;
            break;
        case "sll" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 1;
            inst_info.f7 = 0;
            result_inst.type_block = &inst_info;
            break;
        case "srl" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 5;
            inst_info.f7 = 0;
            result_inst.type_block = &inst_info;
            break;
        case "sra" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 5;
            inst_info.f7 = 32;
            result_inst.type_block = &inst_info;
            break;
        case "slt" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 2;
            inst_info.f7 = 0;
            result_inst.type_block = &inst_info;
            break;
        case "sltu" :
            result_inst.type = "R";
            r_type* inst_info = (r_type*)malloc(sizeof(r_type));
            inst_info.opcode = 51;
            fscanf(f, "%c%d%c %c%d%c %c%d", trash[0], inst_info.rd, trash[1], trash[2], inst_info.rs1, trash[3], trash[4], inst_info.rs2);
            inst_info.f3 = 3;
            inst_info.f7 = 0;
            result_inst.type_block = &inst_info;
            break;
    }


}

int main() {
    inst inst_out = inst_read("test.txt");
    printf("x%d, x%d, x%d\n")
    return 0;
}