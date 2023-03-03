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
	printf("?\t-- display help menu\n");
	printf("quit\t-- exit the simulator\n\n");
	printf("------------------------------------------------------------------\n\n");
}

/***************************************************************/
/* Turn a byte to a word                                                                          */
/***************************************************************/
uint32_t byte_to_word(uint8_t byte)
{
    return (byte & 0x80) ? (byte | 0xffffff80) : byte;
}

/***************************************************************/
/* Turn a halfword to a word                                                                          */
/***************************************************************/
uint32_t half_to_word(uint16_t half)
{
    return (half & 0x8000) ? (half | 0xffff8000) : half;
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
/***********************************3413****************************/
void cycle() {                                                
	handle_instruction();
	CURRENT_STATE = NEXT_STATE;
	INSTRUCTION_COUNT++;
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

/**************************************************************rdump*/
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
			runAll(); 
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

/**************************************************************/
/* Process instructions                                                                                      */
/**************************************************************/
void R_Processing(uint32_t rd, uint32_t f3, uint32_t rs1, uint32_t rs2, uint32_t f7) {
	switch(f3){
		case 0:
			switch(f7){
				case 0:		//add
					NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] + NEXT_STATE.REGS[rs2];
					break;
				case 32:	//sub
					NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] - NEXT_STATE.REGS[rs2];
					break;
				default:
					RUN_FLAG = FALSE;
					break;
				}	
			break;
		case 1: 			//sll
			NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] << NEXT_STATE.REGS[rs2];
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
					NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] >> NEXT_STATE.REGS[rs2];
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
			NEXT_STATE.REGS[rd] = (NEXT_STATE.REGS[rs1] | NEXT_STATE.REGS[rs2]);
			break;
		case 7:				//and
			NEXT_STATE.REGS[rd] = (NEXT_STATE.REGS[rs1] & NEXT_STATE.REGS[rs2]);
			break;
		default:
			RUN_FLAG = FALSE;
			break;
	} 			
}

void ILoad_Processing(uint32_t rd, uint32_t f3, uint32_t rs1, uint32_t imm) {
	switch (f3)
	{
	case 0: //lb
		NEXT_STATE.REGS[rd] = byte_to_word((mem_read_32(NEXT_STATE.REGS[rs1] + imm)) & 0xFF);
		break;

	case 1: //lh
		NEXT_STATE.REGS[rd] = half_to_word((mem_read_32(NEXT_STATE.REGS[rs1] + imm)) & 0xFFFF);
		break;

	case 2: //lw
		NEXT_STATE.REGS[rd] = mem_read_32(NEXT_STATE.REGS[rs1] + imm);
		break;
	
	default:
		printf("Invalid instruction");
		RUN_FLAG = FALSE;
		break;
	}
}

void Iimm_Processing(uint32_t rd, uint32_t f3, uint32_t rs1, uint32_t imm) {
	uint32_t imm0_4 = (imm << 7) >> 7;
	uint32_t imm5_11 = imm >> 5;

	if((imm >> 10) == 1){
			uint32_t Shift =0xFFFFFFFF;
			Shift = Shift << 12;
			imm = Shift + imm;
		}


	switch (f3)
	{
	case 0: //addi
		NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] + (int)imm;
		break;

	case 4: //xori
		NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] ^ (int)imm;
		break;
	
	case 6: //ori
		NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] | (int)imm;
		break;
	
	case 7: //andi
		NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] & (int)imm;
		break;
	
	case 1: //slli
		NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] << imm0_4;
		break;
	
	case 5: //srli and srai
		switch (imm5_11)
		{
		case 0: //srli
			NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] >> imm0_4;
			break;

		case 32: //srai
			printf("\nSRAI NOT IMPLEMENTED YET\n");
			//NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rs1] >> imm0_4;
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
		printf("Invalid instruction");
		RUN_FLAG = FALSE;
		break;
	}
}

void S_Processing(uint32_t imm4, uint32_t f3, uint32_t rs1, uint32_t rs2, uint32_t imm11) {
	// Recombine immediate
	uint32_t imm = (imm11 << 5) + imm4;

	switch (f3)
	{
	case 0: //sb
		mem_write_32((NEXT_STATE.REGS[rs1] + imm), NEXT_STATE.REGS[rs2]);
		break;
	
	case 1: //sh
		mem_write_32((NEXT_STATE.REGS[rs1] + imm), NEXT_STATE.REGS[rs2]);
		break;

	case 2: //sw
		mem_write_32((NEXT_STATE.REGS[rs1] + imm), NEXT_STATE.REGS[rs2]);
		break;

	default:
		printf("Invalid instruction");
		RUN_FLAG = FALSE;
		break;
	}
}

void B_Processing(uint32_t imm4_11, uint32_t f3, uint32_t rs1, uint32_t rs2, uint32_t imm12_5) {
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
		if(NEXT_STATE.REGS[rs1] == NEXT_STATE.REGS[rs2]){
			
			NEXT_STATE.PC += (4*(int)immFull);
		}
		break;
	
	case 1: //bne
		if(NEXT_STATE.REGS[rs1] != NEXT_STATE.REGS[rs2]){
			
			NEXT_STATE.PC += (4*(int)immFull);
		}
		break;

	case 4: //blt
		if(NEXT_STATE.REGS[rs1] < NEXT_STATE.REGS[rs2]){
			
			NEXT_STATE.PC += (4*(int)immFull);
		}
		break;

	case 5:	//bge
		if(NEXT_STATE.REGS[rs1] >= NEXT_STATE.REGS[rs2]){
			
			NEXT_STATE.PC += (4*(int)immFull);
		}
		break;

	case 6: //bltu
		if(NEXT_STATE.REGS[rs1] < NEXT_STATE.REGS[rs2]){
			
			NEXT_STATE.PC += 4*imm12_1;
		}
		break;

	case 7: //bgeu
		if(NEXT_STATE.REGS[rs1] >= NEXT_STATE.REGS[rs2]){
			
			NEXT_STATE.PC += 4*imm12_1;
		}
		break;

	default:
		printf("Invalid instruction");
		RUN_FLAG = FALSE;
		break;
	}
}

void J_Processing() {
	// hi
}

void U_Processing() {
	// hi
}

/************************************************************/
/* decode and execute instruction                                                                     */ 
/************************************************************/
void handle_instruction()
{
	uint32_t addr = CURRENT_STATE.PC;
	uint32_t instruction = mem_read_32(addr);
	uint32_t opcode = (instruction << 25) >> 25;
	uint32_t rd, f3, rs1, rs2, f7, imm, imm4, imm11;
	
	switch(opcode){
		case 51:		//R-type
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			f7 = instruction >> 25;
			R_Processing(rd, f3, rs1, rs2, f7);	
			break;		
		case 19:		//I
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			imm = instruction >> 20;
			imm11 = instruction >> 25;
			Iimm_Processing(rd, f3, rs1, imm);
			break;
		case 3:			//I-type loads
			rd = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			imm = instruction >> 20; 		
			ILoad_Processing(rd, f3, rs1, imm);
			break;
		case 35:		//S
			imm4 = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			imm11 = instruction >> 25;
			S_Processing(imm4, f3, rs1, rs2, imm11);
			break;
		case 99:		//B
			imm4 = (instruction << 20) >> 27;
			f3 = (instruction << 17) >> 29;
			rs1 = (instruction << 12) >> 27;
			rs2 = (instruction << 7) >> 27;
			imm11 = instruction >> 25;
			B_Processing(imm4, f3, rs1, rs2, imm11);
			break;
		case 111:		//jal
			J_Processing();
			break;
		case 103:		//jalr
			break;
		default:
			RUN_FLAG = FALSE;
			break;
	}
	
	NEXT_STATE.PC += 4;

	/*IMPLEMENT THIS*/
	/* execute one instruction at a time. Use/update CURRENT_STATE and and NEXT_STATE, as necessary.*/
}


/************************************************************/
/* Initialize Memory                                                                                                    */ 
/************************************************************/
void initialize() { 
	init_memory();
	CURRENT_STATE.PC = MEM_TEXT_BEGIN;
	CURRENT_STATE.REGS[2] = MEM_STACK_BEGIN;
	NEXT_STATE = CURRENT_STATE;
	RUN_FLAG = TRUE;
}

/************************************************************/
/* Print the program loaded into memory (in RISCV assembly format)    */ 
/************************************************************/
void print_program(){
	int i;
	uint32_t addr;
	
	for(i=0; i<PROGRAM_SIZE; i++){
		addr = MEM_TEXT_BEGIN + (i*4);
		printf("[0x%x]\t", addr);
		print_instruction(addr);
	}
}


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
		printf("Invalid instruction");
		RUN_FLAG = FALSE;
		break;
	}
}

void Iimm_Print(uint32_t rd, uint32_t f3, uint32_t rs1, uint32_t imm) {
	uint32_t imm0_4 = (imm << 7) >> 7;
	uint32_t imm5_11 = imm >> 5;
	switch (f3)
	{
	case 0: //addi
		printf("addi r%d, r%d, %d\n", rd, rs1, imm);
		break;

	case 4: //xori
		printf("xori r%d, r%d, %d\n", rd, rs1, imm);
		break;
	
	case 6: //ori
		printf("ori r%d, r%d, %d\n", rd, rs1, imm);
		break;
	
	case 7: //andi
		printf("andi r%d, r%d, %d\n", rd, rs1, imm);
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
		printf("Invalid instruction");
		RUN_FLAG = FALSE;
		break;
	}
}

void S_Print(uint32_t imm4, uint32_t f3, uint32_t rs1, uint32_t rs2, uint32_t imm11) {
	// Recombine immediate
	uint32_t imm = (imm11 << 5) + imm4;

	switch (f3)
	{
	case 0: //sb
		printf("sb r%d, %d(r%d)\n", rs2, imm, rs1);
		break;
	
	case 1: //sh
		printf("sh r%d, %d(r%d)\n", rs2, imm, rs1);
		break;

	case 2: //sw
		printf("sw r%d, %d(r%d)\n", rs2, imm, rs1);
		break;

	default:
		printf("Invalid instruction");
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
		printf("Invalid instruction");
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