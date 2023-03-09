#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "riscv-inst-read.h"

char byteToHex(uint8_t byte);
void RtypeToHex(r_type * instStruct, char * instruction);
void ItypeToHex(i_type * instStruct, char * instruction);
void StypeToHex(s_type * instStruct, char * instruction);
void BtypeToHex(b_type * instStruct, char * instruction);
char * encodeInstruction(inst instStruct, char * instruction);
