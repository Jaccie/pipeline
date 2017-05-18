#include <stdio.h>
#include "struct.h"
#include <string.h>

FILE* snapshot;
int buff[32];
int reg[32];
int HI_reg,LO_reg,HI,LO;
command comm[258];

void print_cyc(int cycle){
	fprintf(snapshot , "cycle %d\n" , cycle);
	//printf("cycle %d\n",cycle);
}

void print_pc(unsigned int PC){
	fprintf(snapshot , "PC: 0x%08X\n" , PC);
	//printf("PC: 0x%08X\n",PC);
}

void print_IF(int inst){
	fprintf(snapshot , "IF: 0x%08X" , comm[inst].instruction);
	//printf("IF: 0x%08X" , comm[inst].instruction);
}

void print_IFstall(int inst){
	fprintf(snapshot , "IF: 0x%08X to_be_stalled" , comm[inst].instruction);
	//printf("IF: 0x%08X to_be_stalled" , comm[inst].instruction);
}

void print_IFflush(int inst){
	fprintf(snapshot , "IF: 0x%08X to_be_flushed" , comm[inst].instruction);
	//printf("IF: 0x%08X to_be_flushed" , comm[inst].instruction);
}
	
void print_ID(int num){
	fprintf(snapshot , "\nID: ");
	for(int i = 0 ; i < strlen(comm[num].name) ; i++)
		fprintf(snapshot , "%c" , comm[num].name[i]);
	/*printf("\nID: ");
	for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                printf("%c" , comm[num].name[i]);
*/}

void print_IDstall(int num){
	fprintf(snapshot , "\nID: ");
	for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                fprintf(snapshot , "%c" , comm[num].name[i]);
	fprintf(snapshot , " to_be_stalled");
	/*printf("\nID: ");
	for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                printf("%c" , comm[num].name[i]);
        printf(" to_be_stalled");
*/}

void print_IDfwd_rt(int reg){
	fprintf(snapshot , " fwd_EX-DM_rt_$%d",reg);
}

void print_IDfwd_rs(int reg){
        fprintf(snapshot , " fwd_EX-DM_rs_$%d",reg);
}

void print_EX(int num){
	fprintf(snapshot , "\nEX: ");
	for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                fprintf(snapshot , "%c" , comm[num].name[i]);
	/*printf("\nEX: ");
        for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                printf("%c" , comm[num].name[i]);
*/}

void print_EXstall(int num){
	fprintf(snapshot , "\nEX: ");
        for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                fprintf(snapshot , "%c" , comm[num].name[i]);
        fprintf(snapshot , " to_be_stalled");
	/*printf("\nEX: ");
        for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                printf("%c" , comm[num].name[i]);
        printf(" to_be_stalled");
*/}

void print_DM(int num){
	fprintf(snapshot , "\nDM: ");
        for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                fprintf(snapshot , "%c" , comm[num].name[i]);
	/*printf("\nDM: ");
	for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                printf("%c" , comm[num].name[i]);
*/}

void print_WB(int num){
	fprintf(snapshot , "\nWB: ");
        for(int i = 0 ; i < strlen(comm[num].name) ; i++)
                fprintf(snapshot , "%c" , comm[num].name[i]);
	/*printf("\nWB: ");
        for(int i = 0 ; i < strlen(comm[num].name) ; i++)
               printf("%c" , comm[num].name[i]);
*/}

void print_reg(){
	for(int i = 0 ; i < 32 ; i ++){
		if(buff[i] != reg[i]){
			fprintf(snapshot , "$%02d: 0x%08X\n" , i , buff[i]);
			//printf("$%02d: 0x%08X\n" , i , buff[i]);
			reg[i] = buff[i];
		}
	}
	if(HI_reg != HI){
		fprintf(snapshot , "$HI: 0x%08X\n" , HI_reg);
                //printf("$HI: 0x%08X\n" , HI_reg);
                HI = HI_reg;
	}
	if(LO_reg != LO){
		fprintf(snapshot , "$LO: 0x%08X\n" , LO_reg);
                //printf("$LO: 0x%08X\n" , LO_reg);
                LO = LO_reg;
	}
}
