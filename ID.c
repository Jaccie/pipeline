#include <stdio.h>
#include "struct.h"

command comm[258];
int flush;
int ID_EX;
int EX_DM;
int ID_Stall;
int inst_2;
int inst_3;
int fwd_EX_DM_rt_toID;
int fwd_EX_DM_rs_toID;
int reg_rt;
int reg_rs;
int buff[32];

void ID(int i){
	//printf("-------------ID(%d)-------------\n",i);
	ID_Stall = 0;
	fwd_EX_DM_rt_toID = 0;
        fwd_EX_DM_rs_toID = 0;
        flush = 0;

	if(i>=2){
		comm[i].opcode = comm[i].instruction >> 26;
	//rType
	if(comm[i].opcode == 0){
		comm[i].rs = (comm[i].instruction << 6) >> 27;
    		comm[i].rt = (comm[i].instruction << 11) >> 27;
    		comm[i].rd = (comm[i].instruction << 16) >> 27;
    		comm[i].shamt = (comm[i].instruction << 21) >> 27;
    		comm[i].func = (comm[i].instruction << 26) >> 26;
		
		comm[i].halt = 0;
		comm[i].RegWrite = 1;
		comm[i].Branch = 0;
		comm[i].RegDst = 1;
		comm[i].MemRead = 0;
		comm[i].MemtoReg = 0;
		comm[i].MemWrite = 0;
		comm[i].ALUSrc = 0;
		comm[i].nop = 0;
		reg_rt = reg[comm[i].rt];
		reg_rs = reg[comm[i].rs];
		switch(comm[i].func){
			case 32:
				comm[i].name = "ADD";
			break;
			case 33:
				comm[i].name = "ADDU";
			break;
			case 34:
				comm[i].name = "SUB";
			break;
			case 36:
				comm[i].name = "AND";
			break;
			case 37:
				comm[i].name = "OR";
				break;
			case 38:
				comm[i].name = "XOR";
			break;
			case 39:
				comm[i].name = "NOR";
			break;
			case 40:
				comm[i].name = "NAND";
			break;
			case 42:
				comm[i].name = "SLT";
			break;
			case 0:
				comm[i].name = "SLL";
				if(comm[i].rt == 0 && comm[i].rd == 0 && comm[i].shamt == 0){
					comm[i].nop = 1;
					comm[i].RegWrite = 0;
					comm[i].RegDst = 0;
					comm[i].name = "NOP";
				}
			break;
			case 2:
				comm[i].name = "SRL";
			break;
			case 3:
				comm[i].name = "SRA";
			break;
			case 8:
				comm[i].name = "JR";
				comm[i].RegWrite = 0;
				comm[i].Branch = 1;
				
			break;
			case 24:
				comm[i].name = "MULT";
			break;
			case 25:
				comm[i].name = "MULTU";
			break;
			case 16:
				comm[i].name = "MFHI";
			break;
			case 18:
				comm[i].name = "MFLO";
			break;
		}
	}
	//iType
	else if(comm[i].opcode == 8 || comm[i].opcode == 9 || comm[i].opcode == 35 || comm[i].opcode == 33 || comm[i].opcode == 37 || comm[i].opcode == 32 || comm[i].opcode == 36 || comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43 || comm[i].opcode == 15 || comm[i].opcode == 12 || comm[i].opcode == 13 || comm[i].opcode == 14 || comm[i].opcode == 10 || comm[i].opcode == 4 || comm[i].opcode == 5 || comm[i].opcode == 7){
		comm[i].rs = (comm[i].instruction << 6) >> 27;
		comm[i].rt = (comm[i].instruction << 11) >> 27;
		comm[i].C = (comm[i].instruction << 16) >> 16;

		comm[i].halt = 0;
		comm[i].nop = 0;
		comm[i].RegWrite = 1;
                comm[i].Branch = 0;
                comm[i].RegDst = 0;
                comm[i].MemRead = 0;
                comm[i].MemtoReg = 0;
                comm[i].MemWrite = 0;
                comm[i].ALUSrc = 1;
		reg_rt = reg[comm[i].rt];
		reg_rs = reg[comm[i].rs];
		switch(comm[i].opcode){
			case 8:
				comm[i].name = "ADDI";
				
			break;
			case 9:
				comm[i].name = "ADDIU";
			break;
			case 35:
				comm[i].name = "LW";
				comm[i].MemRead = 1;
				comm[i].MemtoReg = 1;
				break;
			case 33:
				comm[i].name = "LH";
				comm[i].MemRead = 1;
				comm[i].MemtoReg = 1;
			break;
			case 37:
                                comm[i].name = "LHU";
                                comm[i].MemRead = 1;
                                comm[i].MemtoReg = 1;
			break;
			case 32:
				comm[i].name = "LB";
                                comm[i].MemRead = 1;
                                comm[i].MemtoReg = 1;
                        break;
			case 36:
				comm[i].name = "LBU";
                                comm[i].MemRead = 1;
                                comm[i].MemtoReg = 1;
                        break;
			case 43:
				comm[i].name = "SW";
				comm[i].RegWrite = 0;
				comm[i].MemWrite = 1;
			break;
			case 41:
				comm[i].name = "SH";
                                comm[i].RegWrite = 0;
                                comm[i].MemWrite = 1;
                        break;
			case 40:
				comm[i].name = "SB";
                                comm[i].RegWrite = 0;
                                comm[i].MemWrite = 1;
                        break;
			case 15:
				comm[i].name = "LUI";
			break;
			case 12:
				comm[i].name = "ANDI";
			break;
			case 13:
				comm[i].name = "ORI";
			break;
			case 14:
				comm[i].name = "NORI";
			break;
			case 10:
				comm[i].name = "SLTI";
			break;
			case 4:
				comm[i].name = "BEQ";
				comm[i].Branch = 1;
				comm[i].RegWrite = 0;
				comm[i].ALUSrc = 0;
				
				//printf("BEQ\nrs: %d\nrt: %d\n",comm[i].rs,comm[i].rt);
					
			break;
			case 5:
				comm[i].name = "BNE";
                                comm[i].Branch = 1;
                                comm[i].RegWrite = 0;
                                comm[i].ALUSrc = 0;
				
				//printf("BNE\nrs: %d\nrt: %d\n",comm[i].rs,comm[i].rt);
                        break;
			case 7:
				comm[i].name = "BGTZ";
                                comm[i].Branch = 1;
                                comm[i].RegWrite = 0;
                                comm[i].ALUSrc = 0;
                        break;
		}
	}
	//jType
	else if(comm[i].opcode == 2 || comm[i].opcode == 3){
		comm[i].jump = (comm[i].instruction << 6) >> 6;
		comm[i].nop = 0;
		comm[i].halt = 0;
                comm[i].RegWrite = 0;
                comm[i].Branch = 1;
                comm[i].RegDst = 0;
                comm[i].MemRead = 0;
                comm[i].MemtoReg = 0;
                comm[i].MemWrite = 0;
                comm[i].ALUSrc = 0;

		if(comm[i].opcode == 2){
			comm[i].name = "J";
		}
		else{
			comm[i].name = "JAL";
			comm[i].RegWrite = 1;                    
		}
	}
	//halt
	else if(comm[i].opcode ==63){
		comm[i].halt = 1;
		comm[i].RegWrite = 0;
                comm[i].Branch = 0;
                comm[i].RegDst = 0;
                comm[i].MemRead = 0;
                comm[i].MemtoReg = 0;
                comm[i].MemWrite = 0;
                comm[i].ALUSrc = 0;
		comm[i].nop = 0;
		comm[i].name = "HALT";
	}
	
	/////////////////////////detect stall in ID!!///////////////////////////
	//lw,lh,lhu,lb,lbu is now in EX
	if(comm[inst_2].MemRead == 1){
		//for rType in ID(no jr,mfhi.mflo)
		if(comm[i].nop == 0 && comm[i].opcode == 0 && comm[i].func != 8 && comm[i].func != 16 && comm[i].func != 18){
			//only use rt
			
			if(comm[i].func == 0 || comm[i].func == 2 || comm[i].func == 3 || comm[i].func == 8){
				if((comm[i].rt == comm[inst_2].rt) && (comm[inst_2].rt != 0)) ID_Stall = 1;
			}
			//use both rt and rs
			else {
				if((comm[i].rt == comm[inst_2].rt || comm[i].rs == comm[inst_2].rt) && (comm[inst_2].rt != 0)) ID_Stall = 1;
			}
	//		printf("stallIDDDDDD= %d\n",ID_Stall);
		}
		//for iType in ID(except LUI)
		else{
			if((comm[i].RegWrite == 1) && (comm[i].opcode != 15) && (comm[i].opcode != 3)){
				if((comm[i].rs == comm[inst_2].rt) && (comm[i].rs != 0)) ID_Stall = 1;
			}
			//sw,sh,sb
			else if(comm[i].MemWrite == 1){
				if((comm[i].rs == comm[inst_2].rt) && (comm[i].rs != 0)) ID_Stall = 1;
				if(comm[i].rt == comm[inst_2].rt && comm[i].rt != 0) ID_Stall = 1;
			}
		}
	}
        //if ID is beq,bne (rs&rt)
	//beq & bne in ID
        if(comm[i].opcode == 4 || comm[i].opcode == 5){
		//for rType in EX
		if(comm[inst_2].nop == 0 && comm[inst_2].opcode == 0 && comm[inst_2].func != 8 && comm[inst_2].func != 24 && comm[inst_2].func != 25 && ((comm[i].rt == comm[inst_2].rd) || (comm[i].rs == comm[inst_2].rd)) && (comm[inst_2].rd != 0)){
			ID_Stall = 1;
		}
                //for iType in EX
                else if((comm[inst_2].ALUSrc == 1) && (comm[inst_2].RegWrite == 1) && ((comm[i].rs == comm[inst_2].rt) || comm[i].rt == comm[inst_2].rt) && (comm[inst_2].rt != 0))
			ID_Stall = 1;
		//for iType in DM(only load)
		else if((comm[inst_3].MemRead == 1) && ((comm[i].rs == comm[inst_3].rt) || comm[i].rt == comm[inst_3].rt) && (comm[inst_3].rt != 0))
                        ID_Stall = 1;
	}
        //if ID is bgtz,jr (rs)
	else if((comm[i].opcode == 0 && comm[i].func  == 8) || (comm[i].opcode == 7)){
		//for rType in EX
		if((comm[inst_2].nop == 0) && (comm[inst_2].opcode == 0) && (comm[i].rs == comm[inst_2].rd) && (comm[i].rs != 0)){
                        	ID_Stall = 1;
		}
                //for iType in EX
                else if((comm[inst_2].ALUSrc == 1) && (comm[inst_2].RegWrite == 1) && (comm[i].rs == comm[inst_2].rt) && (comm[i].rs != 0))
                        ID_Stall = 1;
		//for iType in DM(only load)
		else if((comm[inst_3].MemRead == 1) && (comm[i].rs == comm[inst_3].rt) && (comm[i].rs != 0))
                        ID_Stall = 1;
	}
	//printf("stall = %d\n",ID_Stall);








	////////////////////BRANCH & FOWORD///////////////////////
	if(ID_Stall == 0){
		//jr
		if(comm[i].opcode == 0 && comm[i].func == 8){
			//DM is rType(no jr,mult,multu)
			if(comm[inst_3].nop == 0 && comm[inst_3].opcode == 0 && comm[inst_3].func != 8 && comm[inst_3].func != 24 && comm[inst_3].func != 25 && (comm[i].rs == comm[inst_3].rd) && (comm[inst_3].rd != 0)){
                        	fwd_EX_DM_rs_toID = 1;
				reg_rs = DM_WB;
                	}
			//for iType in DM
               		 else if((comm[inst_3].ALUSrc == 1) && (comm[inst_3].RegWrite == 1) && (comm[i].rs == comm[inst_3].rt) && (comm[inst_3].rt != 0)){
				fwd_EX_DM_rs_toID = 1;
				reg_rs = DM_WB;
			}
			//for jal in DM
			else if(comm[inst_3].opcode == 3 && comm[i].rs == 31){
				fwd_EX_DM_rs_toID = 1;
				reg_rs = DM_WB;
			}
			
			////////////////////solve for use for WB buffers//////////////////
                        //WB is rType(no jr,mult,multu)
                        if(comm[inst_4].nop == 0 && comm[inst_4].opcode == 0 && comm[inst_4].func != 8 && comm[inst_4].func != 24 && comm[inst_4].func != 2){
                                if(comm[inst_4].rd == comm[i].rs && fwd_EX_DM_rs_toID == 0)
                                        reg_rs = buff[comm[i].rs];
                        }
                        //WB is iType(store and branch)
                        else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].RegWrite == 1){
                        //      printf("WB rt$%d, rs$%d\n",comm[inst_4].rt,comm[inst_4].rs);
                                //printf("rt %d == rs %d && fwd_rs %d\n",comm[inst_4].rt,comm[i].rs,fwd_EX_DM_rs_toID);
                                if((comm[inst_4].rt == comm[i].rs) && (fwd_EX_DM_rs_toID == 0)){
                                        reg_rs = buff[comm[i].rs];
                                }
                        }
			//WB is jal
			else if(comm[inst_4].opcode == 3){
				if(comm[i].rs == 31) reg_rs = buff[31];
			}
                	flush = 1;
          	        PC = reg_rs;
                	inst_next = i + (int)(PC-comm[i].address)/4;
            	}
		//beq
		else if(comm[i].opcode == 4){
			//if DM now is rType (no jr,mult,multu)
			if(comm[inst_3].nop == 0 && comm[inst_3].opcode == 0 && comm[inst_3].func != 8 && comm[inst_3].func != 24 && comm[inst_3].func != 25){
	//			printf("FWDBBBEQ\n");
				if((comm[inst_3].rd == comm[i].rt) && (comm[i].rt != 0)){
					fwd_EX_DM_rt_toID = 1;
					reg_rt = DM_WB;
				}
				if((comm[inst_3].rd == comm[i].rs) && (comm[i].rs != 0)){
	 				fwd_EX_DM_rs_toID = 1;
					reg_rs = DM_WB;
				}
			}
			//if DM now is iType(except load, store and branch)
			else if(comm[inst_3].ALUSrc == 1 && comm[inst_3].RegWrite == 1 && comm[inst_3].MemRead == 0){
				if((comm[inst_3].rt == comm[i].rt) && (comm[i].rt != 0)){
					fwd_EX_DM_rt_toID = 1;
					reg_rt = DM_WB;
				}
				if((comm[inst_3].rt == comm[i].rs) && (comm[i].rs != 0)){
					fwd_EX_DM_rs_toID = 1;
					reg_rs = DM_WB;
				}
			}
			//for jal in DM
                        else if(comm[inst_3].opcode == 3){
                                if(comm[i].rt == 31){
					fwd_EX_DM_rt_toID = 1;
                                        reg_rt = DM_WB;
				}
				if(comm[i].rs == 31){
					fwd_EX_DM_rs_toID = 1;
                                        reg_rs = DM_WB;
				}
                        }
			////////////////////solve for use for WB buffers//////////////////
			//WB is rType(no jr,mult,multu)
			if(comm[inst_4].nop == 0 && comm[inst_4].opcode == 0 && comm[inst_4].func != 8 && comm[inst_4].func != 24 && comm[inst_4].func != 2){
				if(comm[inst_4].rd == comm[i].rt && fwd_EX_DM_rt_toID == 0)
					reg_rt = buff[comm[i].rt];
				if(comm[inst_4].rd == comm[i].rs && fwd_EX_DM_rs_toID == 0)
                                        reg_rs = buff[comm[i].rs];
			}
			//WB is iType(store and branch)
			else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].RegWrite == 1){
			//	printf("WB rt$%d, rs$%d\n",comm[inst_4].rt,comm[inst_4].rs);
				if(comm[inst_4].rt == comm[i].rt && fwd_EX_DM_rt_toID == 0){
		                        reg_rt = buff[comm[i].rt];
				}
				//printf("rt %d == rs %d && fwd_rs %d\n",comm[inst_4].rt,comm[i].rs,fwd_EX_DM_rs_toID);
                                if((comm[inst_4].rt == comm[i].rs) && (fwd_EX_DM_rs_toID == 0)){
		                        reg_rs = buff[comm[i].rs];
				}
			}
			//WB is jal
			else if(comm[inst_4].opcode == 3){
				if(comm[i].rt == 31){
                                        reg_rt = buff[31];
                                }
                                if(comm[i].rs == 31){
                                        reg_rs = buff[31];
                                }
			}
			if((comm[i].C>>15)==1)
                                comm[i].C |= 0xffff0000;
                        else
                                comm[i].C &= 0x0000ffff;				
			if((reg_rs == reg_rt)){
				flush = 1;
				PC = PC + 4*comm[i].C;
				inst_next = i + comm[i].C + 1;
               		}
			else PC = PC + 4;
                }
		//bne
		else if(comm[i].opcode == 5){
			//if DM is rType(no jr,mult,multu)
                        if(comm[inst_3].nop == 0 && comm[inst_3].opcode == 0 && comm[inst_3].func != 8 && comm[inst_3].func != 24 && comm[inst_3].func != 25){
                                if((comm[inst_3].rd == comm[i].rt) && (comm[i].rt != 0)){
                                        fwd_EX_DM_rt_toID = 1;
                                        reg_rt = DM_WB;
                                }
                                if((comm[inst_3].rd == comm[i].rs) && (comm[i].rs != 0)){
                                        fwd_EX_DM_rs_toID = 1;
                                        reg_rs = DM_WB;
                                }
                        }
                        //if DM is iType(except load, store and branch)
                        else if(comm[inst_3].ALUSrc == 1 && comm[inst_3].RegWrite == 1 && comm[inst_3].MemRead == 0){
                                if((comm[inst_3].rt == comm[i].rt) && (comm[i].rt != 0)){
                                        fwd_EX_DM_rt_toID = 1;
                                        reg_rt = DM_WB;
                                }
                                if((comm[inst_3].rt == comm[i].rs) && (comm[i].rs != 0)){
                                        fwd_EX_DM_rs_toID = 1;
                                        reg_rs = DM_WB;
                                }
                        }
			//if DM is jal
			else if(comm[inst_3].opcode == 3){
				if(comm[i].rt == 31){
                                	fwd_EX_DM_rt_toID = 1;
                                	reg_rt = DM_WB;
                        	}
                        	if(comm[i].rs == 31){
                                        fwd_EX_DM_rs_toID = 1;
                                        reg_rs = DM_WB;
                                }
			}
			
			////////////////////solve for use for WB buffers//////////////////
                        //WB is rType(no jr,mult,multu)
//                        printf("ID buff[5]:0x%08X\n",buff[5]);
                        if(comm[inst_4].nop == 0 && comm[inst_4].opcode == 0 && comm[inst_4].func != 8 && comm[inst_4].func != 24 && comm[inst_4].func != 2){
                                if(comm[inst_4].rd == comm[i].rt && fwd_EX_DM_rt_toID == 0)
                                        reg_rt = buff[comm[i].rd];
                                if(comm[inst_4].rd == comm[i].rs && fwd_EX_DM_rs_toID == 0)
                                        reg_rs = buff[comm[i].rd];
			}
                        //WB is iType(store and branch)
                        else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].RegWrite == 1){
                        //      printf("WB rt$%d, rs$%d\n",comm[inst_4].rt,comm[inst_4].rs);
                                if(comm[inst_4].rt == comm[i].rt && fwd_EX_DM_rt_toID == 0){
                        	        reg_rt = buff[comm[i].rt];
                                }
                                //printf("rt %d == rs %d && fwd_rs %d\n",comm[inst_4].rt,comm[i].rs,fwd_EX_DM_rs_toID);
                                if((comm[inst_4].rt == comm[i].rs) && (fwd_EX_DM_rs_toID == 0)){
                                        reg_rs = buff[comm[i].rs];
                                }
                        }
			 //WB is jal
                        else if(comm[inst_4].opcode == 3){
                                if(comm[i].rt == 31){
                                        reg_rt = buff[comm[i].rt];
                                }
                                if(comm[i].rs == 31){
                                        reg_rs = buff[comm[i].rs];
                                }
                        }

	
			if((comm[i].C>>15)==1)
                                comm[i].C |= 0xffff0000;
                        else
                                comm[i].C &= 0x0000ffff;

			if(reg_rt != reg_rs){
				flush = 1;
                       		PC = PC + 4*comm[i].C;
                 	        inst_next = i + comm[i].C + 1;
			}
			else PC = PC + 4;
		}
		//bgtz
		else if(comm[i].opcode == 7){
			//if DM is rType(no jr,mult,multu)
                        if(comm[inst_3].nop == 0 && comm[inst_3].opcode == 0 && comm[inst_3].func != 8 && comm[inst_3].func != 24 && comm[inst_3].func != 25){
                                if((comm[inst_3].rd == comm[i].rs) && (comm[i].rs != 0)){
                                        fwd_EX_DM_rs_toID = 1;
                                        reg_rs = DM_WB;
                                }
                        }
                        //if DM is iType(except load, store and branch)
                        else if(comm[inst_3].ALUSrc == 1 && comm[inst_3].RegWrite == 1 && comm[inst_3].MemRead == 0){
                                if((comm[inst_3].rt == comm[i].rs) && (comm[i].rs != 0)){
                                        fwd_EX_DM_rs_toID = 1;
                                        reg_rs = DM_WB;
                                }
                        }
			//if DM is jal
			else if(comm[inst_3].opcode == 3){
				if(comm[i].rs == 31){
					fwd_EX_DM_rs_toID = 1;
                                        reg_rs = DM_WB;
				}
			}
			////////////////////solve for use for WB buffers//////////////////
                        //WB is rType(no jr,mult,multu)
                        if(comm[inst_4].nop == 0 && comm[inst_4].opcode == 0 && comm[inst_4].func != 8 && comm[inst_4].func != 24 && comm[inst_4].func != 25){
                                if(comm[inst_4].rd == comm[i].rs && fwd_EX_DM_rs_toID == 0)
                                        reg_rs = buff[comm[i].rs];
                        }
                        //WB is iType(store and branch)
                        else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].RegWrite == 1){
                        //      printf("WB rt$%d, rs$%d\n",comm[inst_4].rt,comm[inst_4].rs);
                                //printf("rt %d == rs %d && fwd_rs %d\n",comm[inst_4].rt,comm[i].rs,fwd_EX_DM_rs_toID);
                                if((comm[inst_4].rt == comm[i].rs) && (fwd_EX_DM_rs_toID == 0)){
                                reg_rs = buff[comm[i].rs];
                                }

                        }
			//WB is jal
			else if(comm[inst_4].opcode == 3){
				if(comm[i].rs == 31){
					reg_rs = buff[31];
				}
			}
			if((comm[i].C>>15)==1)
                                comm[i].C |= 0xffff0000;
                        else
                                comm[i].C &= 0x0000ffff;

			if(reg_rs > 0){
				flush = 1;
			        PC = PC + 4*comm[i].C;
                        	inst_next = i + comm[i].C + 1;
              		}
			else PC = PC + 4;
		}
		//j
		else if(comm[i].opcode == 2){
			flush = 1;
                        PC = comm[i].jump * 4;
                        inst_next = i + (int)(PC - comm[i].address)/4;
             	}
		//jal
		else if(comm[i].opcode == 3){		
			flush = 1;
			ID_EX = PC;
                        PC = comm[i].jump * 4;
                        inst_next = i + (int)(PC - comm[i].address)/4;
             	}
	}
}//end if(i>=2)
}
