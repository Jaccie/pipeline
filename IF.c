#include <stdio.h>
#include "struct.h"
unsigned int PC;
int ID_Stall;
int inst_next;
int inst_0;
int inst_1;
int inst_2;
int inst_3;
int inst_4;
int end;
command comm[258];
int cycle;

void IF(){
	//printf("-------------IF(%d)------------\n",inst_0);
	if(cycle > 3 && inst_0 >= 2){
		if(comm[inst_0].instruction >> 26 == 63 && comm[inst_1].halt == 1 && comm[inst_2].halt == 1 && comm[inst_3].halt == 1 && comm[inst_4].halt == 1){ 
			isHalt = 1;
			halt();
		}
	}
}


