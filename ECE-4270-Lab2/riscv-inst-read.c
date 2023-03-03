#include "riscv-inst-read.h"

struct inst inst_read(const char* filepath) {
    inst result_inst;
    char inst_name[7];
    char trash[10];

    FILE *f = fopen(filepath, "r");

    fscanf(f, "%s ", inst_name);
    // printf("%s %d", inst_name, strncmp(inst_name, "add",sizeof(char)*7));

    if (strncmp(inst_name, "add",sizeof(char)*7) == 0)
    {
        printf("add ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 0;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "sub",sizeof(char)*7) == 0)
    {
        printf("sub ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 0;
        inst_info->f7 = 32;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "xor",sizeof(char)*7) == 0)
    {
        printf("xor ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 4;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "or",sizeof(char)*7) == 0)
    {
        printf("or ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 6;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "and",sizeof(char)*7) == 0)
    {
        printf("and ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 7;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "sll",sizeof(char)*7) == 0)
    {
        printf("sll ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 1;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "srl",sizeof(char)*7) == 0)
    {
        printf("srl ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 5;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "sra",sizeof(char)*7) == 0)
    {
        printf("sra ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 5;
        inst_info->f7 = 32;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "slt",sizeof(char)*7) == 0)
    {
        printf("slt ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 2;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "sltu", sizeof(char)*7) == 0)
    {
        printf("sltu ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 3;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }

}

int main() {
    inst inst_out = inst_read("test.txt");
    printf("x%d, x%d, x%d\n", ((r_type*)inst_out.type_block)->rd,((r_type*)inst_out.type_block)->rs1,((r_type*)inst_out.type_block)->rs2);
    free(inst_out.type_block);
    return 0;
}