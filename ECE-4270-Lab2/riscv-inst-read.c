#include "riscv-inst-read.h"

struct inst inst_read(FILE * f) {
    inst result_inst;
    char inst_name[7];
    char trash[10];

    //FILE *f = fopen(filepath, "r");

    fscanf(f, "%s ", inst_name);
    // R Type Instructions
    if (strncmp(inst_name, "add",sizeof(char)*7) == 0)
    {
        //printf("add ");
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
        //printf("sub ");
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
        //printf("xor ");
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
        //printf("or ");
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
        //printf("and ");
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
        //printf("sll ");
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
        //printf("srl ");
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
        //printf("sra ");
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
        //printf("slt ");
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
        //printf("sltu ");
        result_inst.type = 'R';
        r_type* inst_info = (r_type*)malloc(sizeof(r_type));
        inst_info->opcode = 51;
        fscanf(f, "%c%d%c %c%d%c %c%d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &trash[4], &inst_info->rs2);
        inst_info->f3 = 3;
        inst_info->f7 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    // I Type Instructions
    if (strncmp(inst_name, "addi", sizeof(char)*7) == 0)
    {
        //printf("addi ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "xori", sizeof(char)*7) == 0)
    {
        //printf("xori ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 4;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "ori", sizeof(char)*7) == 0)
    {
        //printf("ori ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 6;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "andi", sizeof(char)*7) == 0)
    {
        //printf("andi ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 7;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "slli", sizeof(char)*7) == 0)
    {
        //printf("slli ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 1;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "srli", sizeof(char)*7) == 0)
    {
        //printf("srli ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 5;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "srai", sizeof(char)*7) == 0)
    {
        //printf("srai ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 5;
        inst_info->imm = (32 << 5) + inst_info->imm;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "slti", sizeof(char)*7) == 0)
    {
        //printf("slti ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 2;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "sltiu", sizeof(char)*7) == 0)
    {
        //printf("sltiu ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 19;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rd, &trash[1], &trash[2], &inst_info->rs1, &trash[3], &inst_info->imm);
        inst_info->f3 = 3;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "lb", sizeof(char)*7) == 0)
    {
        //printf("lb ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 3;
        fscanf(f, "%c%d%c %d%c%c%d%c", &trash[0], &inst_info->rd, &trash[1], &inst_info->imm, &trash[2], &trash[3], &inst_info->rs1, &trash[4]);
        inst_info->f3 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "lh", sizeof(char)*7) == 0)
    {
        //printf("lh ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 3;
        fscanf(f, "%c%d%c %d%c%c%d%c", &trash[0], &inst_info->rd, &trash[1], &inst_info->imm, &trash[2], &trash[3], &inst_info->rs1, &trash[4]);
        inst_info->f3 = 1;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "lw", sizeof(char)*7) == 0)
    {
        //printf("lw ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 3;
        fscanf(f, "%c%d%c %d%c%c%d%c", &trash[0], &inst_info->rd, &trash[1], &inst_info->imm, &trash[2], &trash[3], &inst_info->rs1, &trash[4]);
        inst_info->f3 = 2;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "lbu", sizeof(char)*7) == 0)
    {
        //printf("lbu ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 3;
        fscanf(f, "%c%d%c %d%c%c%d%c", &trash[0], &inst_info->rd, &trash[1], &inst_info->imm, &trash[2], &trash[3], &inst_info->rs1, &trash[4]);
        inst_info->f3 = 4;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "lhu", sizeof(char)*7) == 0)
    {
        //printf("lhu ");
        result_inst.type = 'I';
        i_type* inst_info = (i_type*)malloc(sizeof(i_type));
        inst_info->opcode = 3;
        fscanf(f, "%c%d%c %d%c%c%d%c", &trash[0], &inst_info->rd, &trash[1], &inst_info->imm, &trash[2], &trash[3], &inst_info->rs1, &trash[4]);
        inst_info->f3 = 5;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    // S Type Instructions
    if (strncmp(inst_name, "sb", sizeof(char)*7) == 0)
    {
        //printf("sb ");
        result_inst.type = 'S';
        s_type* inst_info = (s_type*)malloc(sizeof(s_type));
        inst_info->opcode = 35;
        fscanf(f, "%c%d%c %d%c%c%d%c", &trash[0], &inst_info->rs2, &trash[1], &inst_info->imm, &trash[2], &trash[3], &inst_info->rs1, &trash[4]);
        inst_info->f3 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "sh", sizeof(char)*7) == 0)
    {
        //printf("sh ");
        result_inst.type = 'S';
        s_type* inst_info = (s_type*)malloc(sizeof(s_type));
        inst_info->opcode = 35;
        fscanf(f, "%c%d%c %d%c%c%d%c", &trash[0], &inst_info->rs2, &trash[1], &inst_info->imm, &trash[2], &trash[3], &inst_info->rs1, &trash[4]);
        inst_info->f3 = 1;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "sw", sizeof(char)*7) == 0)
    {
        //printf("sw ");
        result_inst.type = 'S';
        s_type* inst_info = (s_type*)malloc(sizeof(s_type));
        inst_info->opcode = 35;
        fscanf(f, "%c%d%c %d%c%c%d%c", &trash[0], &inst_info->rs2, &trash[1], &inst_info->imm, &trash[2], &trash[3], &inst_info->rs1, &trash[4]);
        inst_info->f3 = 2;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    // B Type Instructions
    if (strncmp(inst_name, "beq", sizeof(char)*7) == 0)
    {
        //printf("beq ");
        result_inst.type = 'B';
        b_type* inst_info = (b_type*)malloc(sizeof(b_type));
        inst_info->opcode = 99;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rs1, &trash[1], &trash[2], &inst_info->rs2, &trash[3], &inst_info->imm);
        inst_info->f3 = 0;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "bne", sizeof(char)*7) == 0)
    {
        //printf("bne ");
        result_inst.type = 'B';
        b_type* inst_info = (b_type*)malloc(sizeof(b_type));
        inst_info->opcode = 99;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rs1, &trash[1], &trash[2], &inst_info->rs2, &trash[3], &inst_info->imm);
        inst_info->f3 = 1;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "blt", sizeof(char)*7) == 0)
    {
        //printf("blt ");
        result_inst.type = 'B';
        b_type* inst_info = (b_type*)malloc(sizeof(b_type));
        inst_info->opcode = 99;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rs1, &trash[1], &trash[2], &inst_info->rs2, &trash[3], &inst_info->imm);
        inst_info->f3 = 4;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "bge", sizeof(char)*7) == 0)
    {
        //printf("bge ");
        result_inst.type = 'B';
        b_type* inst_info = (b_type*)malloc(sizeof(b_type));
        inst_info->opcode = 99;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rs1, &trash[1], &trash[2], &inst_info->rs2, &trash[3], &inst_info->imm);
        inst_info->f3 = 5;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "bltu", sizeof(char)*7) == 0)
    {
        //printf("bltu ");
        result_inst.type = 'B';
        b_type* inst_info = (b_type*)malloc(sizeof(b_type));
        inst_info->opcode = 99;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rs1, &trash[1], &trash[2], &inst_info->rs2, &trash[3], &inst_info->imm);
        inst_info->f3 = 6;
        result_inst.type_block = inst_info;
        return result_inst;
    }
    if (strncmp(inst_name, "bgeu", sizeof(char)*7) == 0)
    {
        //printf("bgeu ");
        result_inst.type = 'B';
        b_type* inst_info = (b_type*)malloc(sizeof(b_type));
        inst_info->opcode = 99;
        fscanf(f, "%c%d%c %c%d%c %d", &trash[0], &inst_info->rs1, &trash[1], &trash[2], &inst_info->rs2, &trash[3], &inst_info->imm);
        inst_info->f3 = 7;
        result_inst.type_block = inst_info;
        return result_inst;
    }

}

/*int main() {
    inst inst_out = inst_read("test.txt");
    printf("x%d, %d(x%d)\n", ((i_type*)inst_out.type_block)->rd,((i_type*)inst_out.type_block)->imm,((i_type*)inst_out.type_block)->rs1);
    free(inst_out.type_block);
    return 0;
}*/