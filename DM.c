#include <stdio.h>
#include "struct.h"

command comm[258];
data d[258];
int EX_DM;
int DM_WB;
int cycle;
int isHalt;
int reg_rt,reg_rs;
int special;
int mf;

void DM(int i){
	isHalt = 0;
	//printf("---------DM------------\n");
	if(i >= 2 && comm[i].nop == 0 && (comm[i].MemRead == 1 || comm[i].MemWrite == 1)){
		////////Detect address overflow & misalignment////////////
		//lw sw
		if(comm[i].opcode == 35 || comm[i].opcode == 43){
			if((EX_DM > 1020) || (EX_DM < 0)){
                                        print_memoverflow(cycle);
                                        isHalt = 1;
                       	}
			if((EX_DM % 4) != 0){
                                        print_misaligned(cycle);
                                        isHalt = 1;
                       	}
		}
		//lh,lhu,sh
		else if(comm[i].opcode == 33 || comm[i].opcode == 37 || comm[i].opcode == 41){
			if((EX_DM > 1022) || (EX_DM < 0)){
                                        print_memoverflow(cycle);
                                        isHalt = 1;
                        }
                                if((EX_DM % 4) != 0 && (EX_DM % 4) != 2){
                                        print_misaligned(cycle);
                                        isHalt = 1;
                        }

		}
		//lb,lbu,sb
		else{
			if((EX_DM > 1024) || (EX_DM < 0)){
				print_memoverflow(cycle);
                                isHalt = 1;
			}
		}

		////////fwd WB buff////////
		//load
		if(isHalt == 0){
		if(comm[i].MemRead == 1){
			reg_rs = comm[i].rs;
			//WB is rType(no jr,mult,multu)
			if(comm[inst_4].opcode == 0 && comm[inst_4].func != 8 && comm[inst_4].func != 24 && comm[inst_4].func != 25){
				if(comm[i].rs == comm[inst_4].rd && comm[i].rs != 0) reg_rs = buff[comm[i].rs];
			}
			//WB is iType(no store and branch)
			else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].MemWrite == 0){
				if(comm[i].rs == comm[inst_4].rt && comm[i].rs != 0) reg_rs = buff[comm[i].rs];
			}
			//WB is jal
			else if(comm[inst_4].opcode == 3){
				if(comm[i].rs == 31) reg_rs = buff[31];
			}
		}
		//store
		else{
			reg_rs = comm[i].rs;
			reg_rt = comm[i].rt;
                        //WB is rType(no jr,mult,multu)
                        if(comm[inst_4].opcode == 0 && comm[inst_4].func != 8 && comm[inst_4].func != 24 && comm[inst_4].func != 25){
                                if(comm[i].rs == comm[inst_4].rd && comm[i].rs != 0) reg_rs = buff[comm[i].rs];
                       		if(comm[i].rt == comm[inst_4].rd && comm[i].rt != 0) reg_rt = buff[comm[i].rt];
			}
                        //WB is iType(no store and branch)
                        else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].MemWrite == 0){
                                if(comm[i].rs == comm[inst_4].rt && comm[i].rs != 0) reg_rs = buff[comm[i].rs];
				if(comm[i].rt == comm[inst_4].rd && comm[i].rt != 0) reg_rt = buff[comm[i].rt];
                        }
                        //WB is jal
                        else if(comm[inst_4].opcode == 3){
                                if(comm[i].rs == 31) reg_rs = buff[31];
				if(comm[i].rt == 31) reg_rt = buff[31];
                        }
		}
		switch(comm[i].opcode){
			case 35: //lw
				DM_WB = d[EX_DM/4 + 2].value;
			break;

			case 33: //lh
				if(EX_DM % 4 == 0) //upper
                                      	DM_WB = d[EX_DM/4 + 2].value >> 16;
				else if(EX_DM % 4 == 2) //lower
					DM_WB = (d[EX_DM/4 + 2].value << 16) >> 16;
				if((DM_WB >> 15) == 1)
					DM_WB = 0xffff0000 | DM_WB;        
				break;

			case 37: //lhu
			 	if(EX_DM % 4 == 0) //upper
                                        DM_WB = d[EX_DM/4 + 2].value >> 16;
                                else if(EX_DM % 4 == 2) //lower
                                        DM_WB = (d[EX_DM/4 + 2].value << 16) >> 16;
			break;
	
			case 32: //lb
				if(EX_DM % 4 == 0) //24-32 bits
					DM_WB = d[EX_DM/4 + 2].value >> 24;
				else if(EX_DM % 4 == 1) //16-24 bits
					DM_WB = (d[EX_DM/4 + 2].value << 8) >> 24;
				else if(EX_DM % 4 == 2) //8-16 bits	
					DM_WB = (d[EX_DM/4 + 2].value << 16) >> 24;
				else //0-8 bits
					 DM_WB = (d[EX_DM/4 + 2].value << 24) >> 24;

				if((DM_WB >> 7) == 1)
					DM_WB = 0xffffff00 | DM_WB;				
			break;

			case 36: //lbu
			        if(EX_DM % 4 == 0) //24-32 bits
                                        DM_WB = d[EX_DM/4 + 2].value >> 24;
                                else if(EX_DM % 4 == 1) //16-24 bits
                                        DM_WB = (d[EX_DM/4 + 2].value << 8) >> 24;
                                else if(EX_DM % 4 == 2) //8-16 bits
                                        DM_WB = (d[EX_DM/4 + 2].value << 16) >> 24;
                                else //0-8 bits
                                        DM_WB = (d[EX_DM/4 + 2].value << 24) >> 24;
                         	
			break;

			case 43: //sw
				if(fwd_EX_DM_rt_toEX == 0)
					d[DM_WB/4 + 2].value = reg[comm[i].rt];
				else
					d[DM_WB/4 + 2].value = special;
			break;

			case 41: //sh
				if(fwd_EX_DM_rt_toEX == 0){
                                       	if(EX_DM % 4 == 0) //upper
                                               	d[EX_DM/4 + 2].value = ((reg[comm[i].rt]  & 0x0000FFFF) << 16) | ((d[EX_DM/4 + 2].value << 16) >> 16);
                                       	else if(EX_DM % 4 == 2) //lower
                                               	d[EX_DM/4 + 2].value = (reg[comm[i].rt] & 0x0000FFFF) | ((d[EX_DM/4 + 2].value >> 16) << 16);
                                }
				else{
					if(EX_DM % 4 == 0) //upper
                                                d[EX_DM/4 + 2].value = ((special & 0x0000FFFF) << 16) | ((d[EX_DM/4 + 2].value << 16) >> 16);
                                        else if(EX_DM % 4 == 2) //lower
                                                d[EX_DM/4 + 2].value = (special & 0x0000FFFF) | ((d[EX_DM/4 + 2].value >> 16) << 16);
				}
                        break;

			case 40: //sb
				if(fwd_EX_DM_rt_toEX == 0){
					if(EX_DM % 4 == 0) //save 24-32 bit
                        			d[EX_DM/4+2].value = (reg[comm[i].rt] & 0X000000FF) << 24 | (d[EX_DM/4+2].value & 0X00FFFFFF);                
					else if(EX_DM % 4 == 1) //save 16-24 bit
                       				d[EX_DM/4+2].value = (reg[comm[i].rt] & 0X000000FF) << 16 | (d[EX_DM/4+2].value & 0XFF00FFFF);
                			else if(EX_DM % 4 == 2) //save 8-16 bit
                       				d[EX_DM/4+2].value = (reg[comm[i].rt] & 0X000000FF) << 8 | (d[EX_DM/4+2].value & 0XFFFF00FF);
                			else  //save 0-8 bit
                       				d[EX_DM/4+2].value = (reg[comm[i].rt] & 0X000000FF) | (d[EX_DM/4+2].value & 0XFFFFFF00);
					
				}
				else{
					if(EX_DM % 4 == 0) //save 24-32 bit
                                                d[EX_DM/4+2].value = (special & 0X000000FF) << 24 | (d[EX_DM/4+2].value & 0X00FFFFFF);
                                        else if(EX_DM % 4 == 1) //save 16-24 bit
                                                d[EX_DM/4+2].value = (special & 0X000000FF) << 16 | (d[EX_DM/4+2].value & 0XFF00FFFF);
                                        else if(EX_DM % 4 == 2) //save 8-16 bit
                                                d[EX_DM/4+2].value = (special & 0X000000FF) << 8 | (d[EX_DM/4+2].value & 0XFFFF00FF);
                                        else  //save 0-8 bit
                                                d[EX_DM/4+2].value = (special & 0X000000FF) | (d[EX_DM/4+2].value & 0XFFFFFF00);

				}
			break;
		}
	}
	else{
		halt();	
	}
	}
	//printf("0x%08X)\n",DM_WB);
}
