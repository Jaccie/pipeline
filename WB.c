#include <stdio.h>
#include "struct.h"

command comm[258];
int EX_DM;
int DM_WB;
int buff[32];
int cycle;

void WB(int i){
	if(i >= 2 && comm[i].nop == 0 && comm[i].RegWrite == 1){
		//rType except mult&multu
		if(comm[i].nop == 0 && comm[i].opcode == 0 && comm[i].func != 24 && comm[i].func != 25){
			if(comm[i].rd == 0) print_writezero(cycle);
			else
				buff[comm[i].rd] = DM_WB;
		}
		//iType
		else if(comm[i].opcode == 8 || comm[i].opcode == 9 || comm[i].opcode == 35 || comm[i].opcode == 33 || comm[i].opcode == 37 || comm[i].opcode == 32 || comm[i].opcode == 36 || comm[i].opcode == 15 || comm[i].opcode == 12 || comm[i].opcode == 13 || comm[i].opcode == 14 || comm[i].opcode == 10){
			if(comm[i].rt == 0) print_writezero(cycle);
			else
				buff[comm[i].rt] = DM_WB;
		}
		//jType
		else if(comm[i].opcode == 3){
			buff[31] = DM_WB;
		}
	}
	DM_WB = EX_DM;
}	
