#include <stdio.h>
//#include "riscv-inst-read.h"
#include "inst-to-hex.h"

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("you're bad\n");
        return 1;
    }
    char * file = argv[1];
    FILE *f = fopen(file, "r");

    char newfile[100] = "hex_";
    strcat(newfile, file);

    FILE * fnew = fopen(newfile, "w");
    while(!feof(f)){
        inst instStruct = inst_read(f);
        char instruction[9];
        encodeInstruction(instStruct, instruction);
        fprintf(fnew, "%s\n", instruction);
    }

    fclose(f);
}