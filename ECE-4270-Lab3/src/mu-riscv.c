#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "mu-riscv.h"

/***************************************************************/
/* Print out a list of commands available                                                                  */
/***************************************************************/
void help() {
	printf("------------------------------------------------------------------\n\n");
	printf("\t**********MU-RISCV Help MENU**********\n\n");
	printf("sim\t-- simulate program to completion \n");
	printf("run <n>\t-- simulate program for <n> instructions\n");
	printf("rdump\t-- dump register values\n");
	printf("reset\t-- clears all registers/memory and re-loads the program\n");
	printf("input <reg> <val>\t-- set GPR <reg> to <val>\n");
	printf("mdump <start> <stop>\t-- dump memory from <start> to <stop> address\n");
	printf("high <val>\t-- set the HI register to <val>\n");
	printf("low <val>\t-- set the LO register to <val>\n");
	printf("print\t-- print the program loaded into memory\n");
	printf("show\t-- print the current content of the pipeline registers\n");
	printf("?\t-- display help menu\n");
	printf("quit\t-- exit the simulator\n\n");
	printf("------------------------------------------------------------------\n\n");
}

/***************************************************************/
/* Read a 32-bit word from memory                                                                            */
/***************************************************************/
uint32_t mem_read_32(uint32_t address)
{
	int i;
	for (i = 0; i < NUM_MEM_REGION; i++) {
		if ( (address >= MEM_REGIONS[i].begin) &&  ( address <= MEM_REGIONS[i].end) ) {
			uint32_t offset = address - MEM_REGIONS[i].begin;
			return (MEM_REGIONS[i].mem[offset+3] << 24) |
					(MEM_REGIONS[i].mem[offset+2] << 16) |
					(MEM_REGIONS[i].mem[offset+1] <<  8) |
					(MEM_REGIONS[i].mem[offset+0] <<  0);
		}
	}
	return 0;
}

/***************************************************************/
/* Write a 32-bit word to memory                                                                                */
/***************************************************************/
void mem_write_32(uint32_t address, uint32_t value)
{
	int i;
	uint32_t offset;
	for (i = 0; i < NUM_MEM_REGION; i++) {
		if ( (address >= MEM_REGIONS[i].begin) && (address <= MEM_REGIONS[i].end) ) {
			offset = address - MEM_REGIONS[i].begin;

			MEM_REGIONS[i].mem[offset+3] = (value >> 24) & 0xFF;
			MEM_REGIONS[i].mem[offset+2] = (value >> 16) & 0xFF;
			MEM_REGIONS[i].mem[offset+1] = (value >>  8) & 0xFF;
			MEM_REGIONS[i].mem[offset+0] = (value >>  0) & 0xFF;
		}
	}
}

/***************************************************************/
/* Execute one cycle                                                                                                              */
/***************************************************************/
void cycle() {
	handle_pipeline();
	CURRENT_STATE = NEXT_STATE;
	CYCLE_COUNT++;
}

/***************************************************************/
/* Simulate RISCV for n cycles                                                                                       */
/***************************************************************/
void run(int num_cycles) {

	if (RUN_FLAG == FALSE) {
		printf("Simulation Stopped\n\n");
		return;
	}

	printf("Running simulator for %d cycles...\n\n", num_cycles);
	int i;
	for (i = 0; i < num_cycles; i++) {
		if (RUN_FLAG == FALSE) {
			printf("Simulation Stopped.\n\n");
			break;
		}
		cycle();
	}
}

/***************************************************************/
/* simulate to completion                                                                                               */
/***************************************************************/
void runAll() {
	if (RUN_FLAG == FALSE) {
		printf("Simulation Stopped.\n\n");
		return;
	}

	printf("Simulation Started...\n\n");
	while (RUN_FLAG){
		cycle();
	}
	printf("Simulation Finished.\n\n");
}

/***************************************************************/
/* Dump a word-aligned region of memory to the terminal                              */
/***************************************************************/
void mdump(uint32_t start, uint32_t stop) {
	uint32_t address;

	printf("-------------------------------------------------------------\n");
	printf("Memory content [0x%08x..0x%08x] :\n", start, stop);
	printf("-------------------------------------------------------------\n");
	printf("\t[Address in Hex (Dec) ]\t[Value]\n");
	for (address = start; address <= stop; address += 4){
		printf("\t0x%08x (%d) :\t0x%08x\n", address, address, mem_read_32(address));
	}
	printf("\n");
}

/***************************************************************/
/* Dump current values of registers to the teminal                                              */
/***************************************************************/
void rdump() {
	int i;
	printf("-------------------------------------\n");
	printf("Dumping Register Content\n");
	printf("-------------------------------------\n");
	printf("# Instructions Executed\t: %u\n", INSTRUCTION_COUNT);
	printf("PC\t: 0x%08x\n", CURRENT_STATE.PC);
	printf("-------------------------------------\n");
	printf("[Register]\t[Value]\n");
	printf("-------------------------------------\n");
	for (i = 0; i < RISCV_REGS; i++){
		printf("[R%d]\t: 0x%08x\n", i, CURRENT_STATE.REGS[i]);
	}
	printf("-------------------------------------\n");
	printf("[HI]\t: 0x%08x\n", CURRENT_STATE.HI);
	printf("[LO]\t: 0x%08x\n", CURRENT_STATE.LO);
	printf("-------------------------------------\n");
}

/***************************************************************/
/* Read a command from standard input.                                                               */
/***************************************************************/
void handle_command() {
	char buffer[20];
	uint32_t start, stop, cycles;
	uint32_t register_no;
	int register_value;
	int hi_reg_value, lo_reg_value;

	printf("MU-RISCV SIM:> ");

	if (scanf("%s", buffer) == EOF){
		exit(0);
	}

	switch(buffer[0]) {
		case 'S':
		case 's':
			if (buffer[1] == 'h' || buffer[1] == 'H'){
				show_pipeline();
			}else {
				runAll();
			}
			break;
		case 'M':
		case 'm':
			if (scanf("%x %x", &start, &stop) != 2){
				break;
			}
			mdump(start, stop);
			break;
		case '?':
			help();
			break;
		case 'Q':
		case 'q':
			printf("**************************\n");
			printf("Exiting MU-RISCV! Good Bye...\n");
			printf("**************************\n");
			exit(0);
		case 'R':
		case 'r':
			if (buffer[1] == 'd' || buffer[1] == 'D'){
				rdump();
			}else if(buffer[1] == 'e' || buffer[1] == 'E'){
				reset();
			}
			else {
				if (scanf("%d", &cycles) != 1) {
					break;
				}
				run(cycles);
			}
			break;
		case 'I':
		case 'i':
			if (scanf("%u %i", &register_no, &register_value) != 2){
				break;
			}
			CURRENT_STATE.REGS[register_no] = register_value;
			NEXT_STATE.REGS[register_no] = register_value;
			break;
		case 'H':
		case 'h':
			if (scanf("%i", &hi_reg_value) != 1){
				break;
			}
			CURRENT_STATE.HI = hi_reg_value;
			NEXT_STATE.HI = hi_reg_value;
			break;
		case 'F':
		case 'f':
			if (scanf("%d", &ENABLE_FORWARDING) != 1) {
				break;
			}
			ENABLE_FORWARDING == 0 ? printf("Forwarding OFF\n") : printf("Forwarding ON\n");
			break;
		case 'L':
		case 'l':
			if (scanf("%i", &lo_reg_value) != 1){
				break;
			}
			CURRENT_STATE.LO = lo_reg_value;
			NEXT_STATE.LO = lo_reg_value;
			break;
		case 'P':
		case 'p':
			print_program();
			break;
		default:
			printf("Invalid Command.\n");
			break;
	}
}

/***************************************************************/
/* reset registers/memory and reload program                                                    */
/***************************************************************/
void reset() {
	int i;
	/*reset registers*/
	for (i = 0; i < RISCV_REGS; i++){
		CURRENT_STATE.REGS[i] = 0;
	}
	CURRENT_STATE.HI = 0;
	CURRENT_STATE.LO = 0;

	for (i = 0; i < NUM_MEM_REGION; i++) {
		uint32_t region_size = MEM_REGIONS[i].end - MEM_REGIONS[i].begin + 1;
		memset(MEM_REGIONS[i].mem, 0, region_size);
	}

	/*load program*/
	load_program();

	/*reset PC*/
	INSTRUCTION_COUNT = 0;
	CURRENT_STATE.PC =  MEM_TEXT_BEGIN;
	NEXT_STATE = CURRENT_STATE;
	RUN_FLAG = TRUE;
}

/***************************************************************/
/* Allocate and set memory to zero                                                                            */
/***************************************************************/
void init_memory() {
	int i;
	for (i = 0; i < NUM_MEM_REGION; i++) {
		uint32_t region_size = MEM_REGIONS[i].end - MEM_REGIONS[i].begin + 1;
		MEM_REGIONS[i].mem = malloc(region_size);
		memset(MEM_REGIONS[i].mem, 0, region_size);
	}
}

/**************************************************************/
/* load program into memory                                                                                      */
/**************************************************************/
void load_program() {
	FILE * fp;
	int i, word;
	uint32_t address;

	/* Open program file. */
	fp = fopen(prog_file, "r");
	if (fp == NULL) {
		printf("Error: Can't open program file %s\n", prog_file);
		exit(-1);
	}

	/* Read in the program. */

	i = 0;
	while( fscanf(fp, "%x\n", &word) != EOF ) {
		address = MEM_TEXT_BEGIN + i;
		mem_write_32(address, word);
		printf("writing 0x%08x into address 0x%08x (%d)\n", word, address, address);
		i += 4;
	}
	PROGRAM_SIZE = i/4;
	printf("Program loaded into memory.\n%d words written into memory.\n\n", PROGRAM_SIZE);
	fclose(fp);
}

/************************************************************/
/* maintain the pipeline                                                                                           */
/************************************************************/
void handle_pipeline()
{
	/*INSTRUCTION_COUNT should be incremented when instruction is done*/
	/*Since we do not have branch/jump instructions, INSTRUCTION_COUNT should be incremented in WB stage */

	WB();
	MEM();
	EX();
	ID();
	IF();
}

/************************************************************/
/* writeback (WB) pipeline stage:                                                                          */
/************************************************************/
void WB()
{
	INSTRUCTION_COUNT++;
	if(INSTRUCTION_COUNT >= PROGRAM_SIZE + 5){
		RUN_FLAG = FALSE;
		return;
	}
	if(INSTRUCTION_COUNT <= 4){
		return;
	}
	uint32_t instruction = MEM_WB.IR;
	uint32_t opcode = (instruction << 25) >> 25;
	switch(opcode){
		case 51:		//R-type
			NEXT_STATE.REGS[MEM_WB.B] = MEM_WB.ALUOutput;
			break;		
		case 19:		//I
			NEXT_STATE.REGS[MEM_WB.B] = MEM_WB.ALUOutput;
			break;
		case 3:			//I-type loads
			NEXT_STATE.REGS[MEM_WB.B] = MEM_WB.LMD;
			break;
		case 35:		//S
			break;
		case 99:		//B -- needs some weird shit technically but nah fam
			break;
		case 111:		//jal
			break;
		case 103:		//jalr
			break;
		default:
			RUN_FLAG = FALSE;
			return;
			break;
	}
	//free(MEM_WB.instName);
	//free instName or chandler will be sad :(
}


uint32_t byte_to_word(uint8_t byte)
{
    return (byte & 0x80) ? (byte | 0xffffff80) : byte;
}

uint32_t half_to_word(uint16_t half)
{
    return (half & 0x8000) ? (half | 0xffff8000) : half;
}


void MEM()
{
	if(INSTRUCTION_COUNT >= PROGRAM_SIZE + 4 || INSTRUCTION_COUNT <= 3) return;
	MEM_WB = EX_MEM;

	char * inst_name = MEM_WB.instName;

	//Load
	if (strncmp(inst_name, "lb", sizeof(char)*7) == 0)
    {
        MEM_WB.LMD = byte_to_word((mem_read_32(MEM_WB.ALUOutput)) & 0xFF);
    }
	if (strncmp(inst_name, "lh", sizeof(char)*7) == 0)
    {
        MEM_WB.LMD = half_to_word((mem_read_32(MEM_WB.ALUOutput)) & 0xFFFF);
    }
	if (strncmp(inst_name, "lw", sizeof(char)*7) == 0)
    {
        MEM_WB.LMD = mem_read_32(MEM_WB.ALUOutput);
    }
	if (strncmp(inst_name, "lbu", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
	if (strncmp(inst_name, "lhu", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
	
	//Store
	if (strncmp(inst_name, "sb", sizeof(char)*7) == 0)
    {
        mem_write_32((MEM_WB.ALUOutput), MEM_WB.B);
    }
	if (strncmp(inst_name, "sh", sizeof(char)*7) == 0)
    {
        mem_write_32((MEM_WB.ALUOutput), MEM_WB.B);
    }
	if (strncmp(inst_name, "sw", sizeof(char)*7) == 0)
    {
        mem_write_32((MEM_WB.ALUOutput), MEM_WB.B);
    }

}

/************************************************************/
/* execution (EX) pipeline stage:                                                                          */
/************************************************************/
void EX()
{
	if(INSTRUCTION_COUNT >= PROGRAM_SIZE + 3 || INSTRUCTION_COUNT <= 2) return;
	EX_MEM = ID_EX;

    char * inst_name = EX_MEM.instName;

    // R Type Instructions
    if (strncmp(inst_name, "add",sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A + EX_MEM.imm;
    }
    if (strncmp(inst_name, "sub",sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A - EX_MEM.imm;
    }
    if (strncmp(inst_name, "xor",sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "or",sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A | EX_MEM.imm;
    }
    if (strncmp(inst_name, "and",sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A & EX_MEM.imm;
    }
    if (strncmp(inst_name, "sll",sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A << EX_MEM.imm;
    }
    if (strncmp(inst_name, "srl",sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A >> EX_MEM.imm;
    }
    if (strncmp(inst_name, "sra",sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "slt",sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "sltu", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    // I Type Instructions
    if (strncmp(inst_name, "addi", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A + EX_MEM.imm;
    }
    if (strncmp(inst_name, "xori", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "ori", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A | EX_MEM.imm;
    }
    if (strncmp(inst_name, "andi", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A & EX_MEM.imm;
    }
    if (strncmp(inst_name, "slli", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A << EX_MEM.imm;
    }
    if (strncmp(inst_name, "srli", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A >> EX_MEM.imm;
    }
    if (strncmp(inst_name, "srai", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "slti", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "sltiu", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "lb", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A + EX_MEM.imm;
    }
    if (strncmp(inst_name, "lh", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A + EX_MEM.imm;
    }
    if (strncmp(inst_name, "lw", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A + EX_MEM.imm;
    }
    if (strncmp(inst_name, "lbu", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "lhu", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    // S Type Instructions
    if (strncmp(inst_name, "sb", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A + EX_MEM.imm;
    }
    if (strncmp(inst_name, "sh", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A + EX_MEM.imm;
    }
    if (strncmp(inst_name, "sw", sizeof(char)*7) == 0)
    {
        EX_MEM.ALUOutput = EX_MEM.A + EX_MEM.imm;
    }
    // B Type Instructions
    if (strncmp(inst_name, "beq", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "bne", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "blt", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "bge", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "bltu", sizeof(char)*7) == 0)
    {
        RUN_FLAG = FALSE;
    }
    if (strncmp(inst_name, "bgeu", sizeof(char)*7) == 0)
    {
       RUN_FLAG = FALSE;
    }

}

/************************************************************/
/* instruction decode (ID) pipeline stage:                                                         */
/************************************************************/
void R_Decode(uint32_t f3, uint32_t f7) {
	switch(f3){
		case 0:
			switch(f7){
				case 0:		//add
					ID_EX.instName = "add";
					break;
				case 32:	//sub
					ID_EX.instName = "sub";
					break;
				default:
					RUN_FLAG = FALSE;
					break;
				}	
			break;
		case 1: 			//sll
			ID_EX.instName = "sll";
			break;
		case 2:				//slt
			RUN_FLAG = FALSE;
			break;
		case 3:				//sltu
			RUN_FLAG = FALSE;
			break;
		case 4:				//xor
			RUN_FLAG = FALSE;
			break;
		case 5:
			switch(f7){
				case 0:		//srl
					ID_EX.instName = "srl";
					break;
				case 32:	//sra
					RUN_FLAG = FALSE;
					break;
				default:
					RUN_FLAG = FALSE;
					break;
				}	
			break;
		case 6: 			//or
			ID_EX.instName = "or";
			break;
		case 7:				//and
			ID_EX.instName = "and";
			break;
		default:
			RUN_FLAG = FALSE;
			break;
	} 			
}

void ILoad_Decode(uint32_t f3) {
	switch (f3)
	{
	case 0: //lb
		ID_EX.instName = "lb";
		break;

	case 1: //lh
		ID_EX.instName = "lh";
		break;

	case 2: //lw
		ID_EX.instName = "lw";
		break;
	
	default:
		printf("Invalid instruction\n");
		RUN_FLAG = FALSE;
		break;
	}
}

void Iimm_Decode(uint32_t f3, uint32_t imm) {
	uint32_t imm5_11 = imm >> 5;
	switch (f3)
	{
	case 0: //addi
		ID_EX.instName = "addi";
		break;

	case 4: //xori
		ID_EX.instName = "xori";
		break;
	
	case 6: //ori
		ID_EX.instName = "ori";
		break;
	
	case 7: //andi
		ID_EX.instName = "andi";
		break;
	
	case 1: //slli
		ID_EX.instName = "slli";
		break;
	
	case 5: //srli and srai
		switch (imm5_11)
		{
		case 0: //srli
			ID_EX.instName = "srli";
			break;

		case 32: //srai
			RUN_FLAG = FALSE;
			break;
		
		default:
			RUN_FLAG = FALSE;
			break;
		}
		break;
	
	case 2:
		break;

	case 3:
		break;

	default:
		printf("Invalid instruction\n");
		RUN_FLAG = FALSE;
		break;
	}
}

void S_Decode(uint32_t imm4, uint32_t f3, uint32_t imm11) {
	// Recombine immediate
	uint32_t imm = (imm11 << 5) + imm4;
	ID_EX.imm = imm;

	switch (f3)
	{
	case 0: //sb
		ID_EX.instName = "sb";
		break;
	
	case 1: //sh
		ID_EX.instName = "sh";
		break;

	case 2: //sw
		ID_EX.instName = "sw";
		break;

	default:
		printf("Invalid instruction\n");
		RUN_FLAG = FALSE;
		break;
	}
}

void B_Decode(uint32_t imm4_11, uint32_t f3, uint32_t imm12_5) {
	//Get full immediate number.
	uint32_t imm12_1 = 0;
	uint32_t immFull = 0;
    uint32_t temp = 0;
    
    temp = (imm4_11 << 27) >> 28;
    imm12_1 += temp;
    
    temp = ((imm12_5 << 26) >> 22);
    imm12_1 += temp;

    temp = ((imm4_11 << 31) >> 21);
    imm12_1 += temp;

    temp = ((imm12_5 >> 6) << 11);
    imm12_1 += temp;

	immFull = imm12_1;
	ID_EX.imm = immFull;

	if((imm12_5 >> 6) == 1){
			uint32_t Shift =0xFFFFFFFF;
			Shift = Shift << 12;
			immFull = Shift + immFull;
		}

	switch (f3)
	{
	case 0: //beq
		ID_EX.instName = "beq";
		break;
	
	case 1: //bne
		ID_EX.instName = "bne";
		break;

	case 4: //blt
		ID_EX.instName = "blt";
		break;

	case 5:	//bge
		ID_EX.instName = "bge";
		break;

	case 6: //bltu
		ID_EX.instName = "bltu";
		break;

	case 7: //bgeu
		ID_EX.instName = "bgeu";
		break;

	default:
		printf("Invalid instruction\n");
		RUN_FLAG = FALSE;
		break;
	}
}

void ID()
{
	if(INSTRUCTION_COUNT >= PROGRAM_SIZE + 2 || INSTRUCTION_COUNT <= 1) return;
	ID_EX = IF_ID;
	ID_EX.instName = malloc(sizeof(char) * 7);
	if(!ID_EX.instName){
		RUN_FLAG = FALSE;
		printf("mawwoc faiwiuwe\n"); //just for you chandler <3
		return;
	}
	uint32_t instruction = ID_EX.IR;
	uint32_t opcode = (instruction << 25) >> 25;
	uint32_t rd, f3, rs1, rs2, f7, imm, imm4, imm11;

	switch(opcode){
		case 51:		//R-type
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			f7 = instruction >> 25;
			ID_EX.A = NEXT_STATE.REGS[rs1];
			ID_EX.imm = NEXT_STATE.REGS[rs2];
			ID_EX.B = rd;
			R_Decode(f3, f7);	
			break;		
		case 19:		//I
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			imm = instruction >> 20;
			imm11 = instruction >> 25;
			ID_EX.A = NEXT_STATE.REGS[rs1];
			ID_EX.B = rd;
			ID_EX.imm = imm;
			Iimm_Decode(f3, imm);
			break;
		case 3:			//I-type loads
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			imm = instruction >> 20; 
			ID_EX.A = NEXT_STATE.REGS[rs1];
			ID_EX.B = rd;
			ID_EX.imm = imm;		
			ILoad_Decode(f3);
			break;
		case 35:		//S
			imm4 = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			imm11 = instruction >> 25;
			ID_EX.A = NEXT_STATE.REGS[rs1];
			ID_EX.B = NEXT_STATE.REGS[rs2];
			S_Decode(imm4, f3, imm11);
			break;
		case 99:		//B -- needs some weird shit technically but nah fam
			imm4 = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			imm11 = instruction >> 25;
			ID_EX.A = NEXT_STATE.REGS[rs1];
			ID_EX.B = NEXT_STATE.REGS[rs2];
			B_Decode(imm4, f3, imm11);
			break;
		case 111:		//jal
			break;
		case 103:		//jalr
			break;
		default:
			RUN_FLAG = FALSE;
			return;
			break;
	}
	if (EX_MEM.ALUOutput & (EX_MEM.imm != 0) & (EX_MEM.imm = ID_EX.A))
	{

	}
	if (EX_MEM.ALUOutput & (EX_MEM.imm != 0) & (EX_MEM.imm = ID_EX.B))
	{

	}
	if (MEM_WB.LMD & (MEM_WB.imm != 0) & (MEM_WB.imm = ID_EX.A))
	{

	}
	if (MEM_WB.LMD & (MEM_WB.imm != 0) & (MEM_WB.imm = ID_EX.B))
	{
		
	}
}

/************************************************************/
/* instruction fetch (IF) pipeline stage:                                                              */
/************************************************************/
void IF()
{
	if(INSTRUCTION_COUNT >= PROGRAM_SIZE + 1) return;
	uint32_t addr = CURRENT_STATE.PC;
	IF_ID.PC = addr;
	uint32_t instruction = mem_read_32(addr);
	IF_ID.IR = instruction;
	NEXT_STATE.PC += 4;
}


/************************************************************/
/* Initialize Memory                                                                                                    */
/************************************************************/
void initialize() {
	init_memory();
	INSTRUCTION_COUNT = 0;
	CURRENT_STATE.PC = MEM_TEXT_BEGIN;
	NEXT_STATE = CURRENT_STATE;
	RUN_FLAG = TRUE;
}

/************************************************************/
/* Print the program loaded into memory (in RISCV assembly format)    */
/************************************************************/

void R_Print(uint32_t rd, uint32_t f3, uint32_t rs1, uint32_t rs2, uint32_t f7) {
	switch(f3){
		case 0:
			switch(f7){
				case 0:		//add
					printf("add r%d, r%d, r%d\n", rd, rs1, rs2);
					break;
				case 32:	//sub
					printf("sub r%d, r%d, r%d\n", rd, rs1, rs2);
					break;
				default:
					printf("WARNING: INSTRUCTION NOT FOUND!\n");
					break;
				}	
			break;
		case 6: 			//or
			printf("or r%d, r%d, r%d\n", rd, rs1, rs2);
			break;
		case 7:				//and
			printf("and r%d, r%d, r%d\n", rd, rs1, rs2);
			break;
		default:
			printf("WARNING: INSTRUCTION NOT FOUND!\n");
			break;
	} 			
}

void ILoad_Print(uint32_t rd, uint32_t f3, uint32_t rs1, uint32_t imm) {
	switch (f3)
	{
	case 0: //lb
		printf("lb r%d, %d(r%d)\n", rd, imm, rs1);
		break;

	case 1: //lh
		printf("lh r%d, %d(r%d)\n", rd, imm, rs1);
		break;

	case 2: //lw
		printf("lw r%d, %d(r%d)\n", rd, imm, rs1);
		break;
	
	default:
		printf("Invalid instruction\n");
		RUN_FLAG = FALSE;
		break;
	}
}

void Iimm_Print(uint32_t rd, uint32_t f3, uint32_t rs1, uint32_t imm) {
	uint32_t imm0_4 = (imm << 7) >> 7;
	uint32_t imm5_11 = imm >> 5;

	if((imm >> 11) == 1){
		uint32_t Shift =0xFFFFFFFF;
		Shift = Shift << 12;
		imm = Shift + imm;
	}

	switch (f3)
	{
	case 0: //addi
		printf("addi r%d, r%d, %d\n", rd, rs1, (int)imm);
		break;

	case 4: //xori
		printf("xori r%d, r%d, %d\n", rd, rs1, (int)imm);
		break;
	
	case 6: //ori
		printf("ori r%d, r%d, %d\n", rd, rs1, (int)imm);
		break;
	
	case 7: //andi
		printf("andi r%d, r%d, %d\n", rd, rs1, (int)imm);
		break;
	
	case 1: //slli
		printf("slli r%d, r%d, %d\n", rd, rs1, imm0_4);
		break;
	
	case 5: //srli and srai
		switch (imm5_11)
		{
		case 0: //srli
			printf("srli r%d, r%d, %d\n", rd, rs1, imm0_4);
			break;

		case 32: //srai
			//NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] >> imm0_4;
			break;
		
		default:
			break;
		}
		break;
	
	case 2:
		break;

	case 3:
		break;

	default:
		printf("Invalid instruction\n");
		RUN_FLAG = FALSE;
		break;
	}
}

void S_Print(uint32_t imm4, uint32_t f3, uint32_t rs1, uint32_t rs2, uint32_t imm11) {
	// Recombine immediate
	uint32_t imm = (imm11 << 5) + imm4;

	if((imm >> 11) == 1){
		uint32_t Shift =0xFFFFFFFF;
		Shift = Shift << 12;
		imm = Shift + imm;
	}

	switch (f3)
	{
	case 0: //sb
		printf("sb r%d, %d(r%d)\n", rs2, (int)imm, rs1);
		break;
	
	case 1: //sh
		printf("sh r%d, %d(r%d)\n", rs2, (int)imm, rs1);
		break;

	case 2: //sw
		printf("sw r%d, %d(r%d)\n", rs2, (int)imm, rs1);
		break;

	default:
		printf("Invalid instruction\n");
		RUN_FLAG = FALSE;
		break;
	}
}

void B_Print(uint32_t imm4_11, uint32_t f3, uint32_t rs1, uint32_t rs2, uint32_t imm12_5) {
	//Get full immediate number.
	uint32_t imm12_1 = 0;
	uint32_t immFull = 0;
    uint32_t temp = 0;
    
    temp = (imm4_11 << 27) >> 28;
    imm12_1 += temp;
    
    temp = ((imm12_5 << 26) >> 22);
    imm12_1 += temp;

    temp = ((imm4_11 << 31) >> 21);
    imm12_1 += temp;

    temp = ((imm12_5 >> 6) << 11);
    imm12_1 += temp;

	immFull = imm12_1;

	if((imm12_5 >> 6) == 1){
			uint32_t Shift =0xFFFFFFFF;
			Shift = Shift << 12;
			immFull = Shift + immFull;
		}

	switch (f3)
	{
	case 0: //beq
		printf("beq r%d, r%d, %d\n", rs1, rs2, (int)immFull);
		break;
	
	case 1: //bne
		printf("bne r%d, r%d, %d\n", rs1, rs2, (int)immFull);
		break;

	case 4: //blt
		printf("blt r%d, r%d, %d\n", rs1, rs2, (int)immFull);
		break;

	case 5:	//bge
		printf("bge r%d, r%d, %d\n", rs1, rs2, (int)immFull);
		break;

	case 6: //bltu
		printf("bltu r%d, r%d, %d\n", rs1, rs2, imm12_1);
		break;

	case 7: //bgeu
		printf("bgeu r%d, r%d, %d\n", rs1, rs2, imm12_1);
		break;

	default:
		printf("Invalid instruction\n");
		RUN_FLAG = FALSE;
		break;
	}
}

void J_Print() {
	// hi
}

void U_Print() {
	// hi
}

/************************************************************/
/* Print the instruction at given memory address (in RISCV assembly format)    */
/************************************************************/
void print_instruction(uint32_t addr){
	uint32_t instruction = mem_read_32(addr);
	//printf("%x\n", instruction);

	uint32_t opcode = (instruction << 25) >> 25;
	uint32_t rd, f3, rs1, rs2, f7, imm, imm4, imm11;
	
	switch(opcode){
		case 51:		//R-type
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			f7 = instruction >> 25;
			R_Print(rd, f3, rs1, rs2, f7);	
			break;		
		case 19:		//I
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			imm = instruction >> 20;
			Iimm_Print(rd, f3, rs1, imm);
			break;
		case 3:			//I-type loads
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			imm = instruction >> 20; 		
			ILoad_Print(rd, f3, rs1, imm);
			break;
		case 35:		//S
			imm4 = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			imm11 = instruction >> 25;
			S_Print(imm4, f3, rs1, rs2, imm11);
			break;
		case 99:		//B
			imm4 = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			imm11 = instruction >> 25;
			B_Print(imm4, f3, rs1, rs2, imm11);
			break;
		case 111:		//jal
			J_Print();
			break;
		case 103:		//jalr
			break;
		default:
			printf("WARNING: INSTRUCTION NOT FOUND!\n");
			break;
	}
}

void print_program(){
	int i;
	uint32_t addr;
	
	for(i=0; i<PROGRAM_SIZE; i++){
		addr = MEM_TEXT_BEGIN + (i*4);
		printf("[0x%x]\t", addr);
		print_instruction(addr);
	}
}

/************************************************************/
/* Print the current pipeline                                                                                    */
/************************************************************/
void show_pipeline(){
	printf("Current PC:\t\t%d\n", NEXT_STATE.PC);
	printf("IF/ID.IR:\t\t%x\n", IF_ID.IR);
	printf("IF/ID.PC:\t\t%d\n\n", IF_ID.PC);
	printf("ID/EX.IR:\t\t%x\n", ID_EX.IR);
	printf("ID/EX.A:\t\t%d\n", ID_EX.A);
	printf("ID/EX.B:\t\t%d\n", ID_EX.B);
	printf("ID/EX.ALUOut:\t\t%d\n\n", ID_EX.ALUOutput);
	printf("EX/MEM.IR:\t\t%x\n", EX_MEM.IR);
	printf("EX/MEM.A:\t\t%d\n", EX_MEM.A);
	printf("EX/MEM.B:\t\t%d\n", EX_MEM.B);
	printf("EX/MEM.ALUOut:\t\t%d\n\n", EX_MEM.ALUOutput);
	printf("MEM/WB.IR:\t\t%x\n", MEM_WB.IR);
	printf("MEM/WB.ALUOut:\t\t%d\n", MEM_WB.ALUOutput);
	printf("MEM/WB.LMD:\t\t%d\n\n", MEM_WB.LMD);
}

/***************************************************************/
/* main                                                                                                                                   */
/***************************************************************/
int main(int argc, char *argv[]) {
	printf("\n**************************\n");
	printf("Welcome to MU-RISCV SIM...\n");
	printf("**************************\n\n");

	if (argc < 2) {
		printf("Error: You should provide input file.\nUsage: %s <input program> \n\n",  argv[0]);
		exit(1);
	}

	strcpy(prog_file, argv[1]);
	initialize();
	load_program();
	help();
	while (1){
		handle_command();
	}
	return 0;
}
