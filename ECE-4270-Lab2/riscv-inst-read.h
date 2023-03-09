#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct inst {
    char type;
    void* type_block;
} inst;

typedef struct r_type {
    char inst_name[7];
    int opcode;
    int f3;
    int f7;
    int rd;
    int rs1;
    int rs2;
} r_type;

typedef struct i_type {
    char inst_name[7];
    int opcode;
    int f3;
    int imm;
    int rd;
    int rs1;
} i_type;

typedef struct s_type {
    char inst_name[7];
    int opcode;
    int f3;
    int rs1;
    int rs2;
    int imm;
} s_type;

typedef struct b_type {
    char inst_name[7];
    int opcode;
    int f3;
    int rs1;
    int rs2;
    int imm;
} b_type;

typedef struct j_type {
    char inst_name[7];
    int opcode;
    int rd;
    int imm;
} j_type;

struct inst inst_read(FILE *);