#include <stdio.h>
#include "struct.h"
#include <string.h>
int cycle;
FILE *snapshot;
command comm[258];
int reg[32];
data d[258];
int buff[32];
unsigned int PC;
int ID_EX;
int EX_reg;
int EX_DM;
int DM_WB;
int inst_next;
int inst_1;
int inst_2;
int inst_3;
int inst_4;
int inst_0;
int end;
int isHalt;
int HI,LO;
int fwd_EX_DM_rt_toID;
int fwd_EX_DM_rs_toID;
int fwd_EX_DM_rt_toEX;
int fwd_EX_DM_rs_toEX;
int fwd_DM_WB_rt_toEX;
int fwd_DM_WB_rs_toEX;
int num_of_instruction;

void pipe(){
	inst_0 = 3;
	inst_1 = 2;
	inst_2 = 0;
	inst_3 = 0;
	inst_4 = 0;
	inst_next = 3;
	cycle = 0;
	end = 0;
	isHalt = 0;
	
	for(int i = 0 ; i < 32 ; i++){
		buff[i] = reg[i];
	}

	print_cyc(cycle);
	cycle ++;
	for(int i = 0 ; i < 32 ; i++){ 
		fprintf(snapshot , "$%02d: 0x%08X\n" , i , reg[i]); 
		//printf("$%02d: 0x%08X\n",i,reg[i]);
	}
	fprintf(snapshot , "$HI: 0x%08X\n$LO: 0x%08X\n" , HI , LO);
	//printf("$HI: 0x%08X\n$LO: 0x%08X\n" , HI , LO);
	print_pc(PC);
	print_IF(2);
        fprintf(snapshot , "\nID: NOP\n"); //printf("\nID: NOP\n");
        fprintf(snapshot , "EX: NOP\n"); //printf("EX: NOP\n");
        fprintf(snapshot , "DM: NOP\n"); //printf("DM: NOP\n");
        fprintf(snapshot , "WB: NOP\n\n\n"); //printf("WB: NOP\n\n\n");
	while(cycle <= 500000 && end == 0 && isHalt == 0){
		print_cyc(cycle);
		cycle ++ ;
		print_reg();
		if(ID_Stall == 0 && inst_0 >= 2){
			if(comm[inst_2].Branch == 0) PC = PC + 4;
		}
		else if(PC < comm[0].instruction && inst_0 < 2 && inst_2 < 2){ 
			PC = PC + 4;
		}
	
		if(ID_Stall == 0) inst_next = inst_next + 1;
		
                print_pc(PC);
		WB(inst_4);
		DM(inst_3);
		EX(inst_2);
		ID(inst_1);
		IF();
		
		if(inst_0 >= 2){
			if(ID_Stall == 0){
				if(flush == 1) print_IFflush(inst_0);
				else print_IF(inst_0);
			}
			else
				print_IFstall(inst_0);
		}
		else{
			//printf("IF: 0x00000000");
			fprintf(snapshot , "IF: 0x00000000");
		}


		if(inst_1 >= 2){
			if(ID_Stall == 1)
				print_IDstall(inst_1);
			else{
				print_ID(inst_1);
				if(fwd_EX_DM_rs_toID == 1){
                               		fprintf(snapshot , " fwd_EX-DM_rs_$%d",comm[inst_1].rs);
                        		//printf(" fwd_EX_DM_rs_$%d",comm[inst_1].rs);
				}
				if(fwd_EX_DM_rt_toID == 1){
                                	fprintf(snapshot , " fwd_EX-DM_rt_$%d",comm[inst_1].rt);
					//printf(" fwd_EX_DM_rt_$%d",comm[inst_1].rt);
				}
			}
		}
		else{
			//printf("\nID: NOP");
                        fprintf(snapshot , "\nID: NOP");
		}
		//EX stage
		if(inst_2 >= 2){
			print_EX(inst_2);                         	
			if(fwd_DM_WB_rs_toEX == 1){
				fprintf(snapshot , " fwd_DM-WB_rs_$%d",comm[inst_2].rs);
				//printf(" fwd_DM-WB_rs_$%d",comm[inst_2].rs);
			}
			if(fwd_EX_DM_rs_toEX == 1){
                                fprintf(snapshot , " fwd_EX-DM_rs_$%d",comm[inst_2].rs);
                                //printf(" fwd_EX_DM_rs_$%d",comm[inst_2].rs);
                        }
			if(fwd_DM_WB_rt_toEX == 1){
                                fprintf(snapshot , " fwd_DM-WB_rt_$%d",comm[inst_2].rt);
				//printf(" fwd_DM-WB_rt_$%d",comm[inst_2].rt);
			}	
                        if(fwd_EX_DM_rt_toEX == 1){
                                fprintf(snapshot , " fwd_EX-DM_rt_$%d",comm[inst_2].rt);
				//printf(" fwd_EX_DM_rt_$%d",comm[inst_2].rt);
			}
		}
		else{
			//printf("\nEX: NOP");
			fprintf(snapshot , "\nEX: NOP");
		}
		//DM stage
		if(inst_3 >= 2)
			print_DM(inst_3);
		else{
			//printf("\nDM: NOP");
			fprintf(snapshot , "\nDM: NOP");
		}
		if(inst_4 >= 2)
			print_WB(inst_4);
		else{
			//printf("\nWB: NOP");
			fprintf(snapshot , "\nWB: NOP");
		}
		//printf("\n\n\n");
		fprintf(snapshot , "\n\n\n");

		if(ID_Stall == 1){
//			printf("STALL  = 1\n");
			inst_4 = inst_3;
			inst_3 = inst_2;
			inst_2 = 0;
			inst_1 = inst_1;
		}
		else if(flush == 1){
			inst_4 = inst_3;
			inst_3 = inst_2;
			inst_2 = inst_1;
			inst_1 = 0;
			inst_0 = inst_next;
			flush  = 0;
		}
		else{
		//if didn't jump
			inst_4 = inst_3;
			inst_3 = inst_2;
			inst_2 = inst_1;
			inst_1 = inst_0;
			inst_0 = inst_next;
		}
		if(inst_0 > num_of_instruction + 4) end = 1;
	}
	if(cycle > 500000) printf("illegal cycles, over 500,000 cycles\n");
	else if(isHalt == 0){
		printf("illegl halt, not all stages filled with halts\n");
	}
}
