#include <stdio.h>
#include "struct.h"

command comm[258];
int EX_DM;
int mf;
int HI_reg,LO_reg;
int cycle;
int ID_DM;
int fwd_EX_DM_rt_toEX;
int fwd_EX_DM_rs_toEX;
int fwd_DM_WB_rt_toEX;
int fwd_DM_WB_rs_toEX;
int special;
int EX_reg;

void EX(int i){
	long long int result = 0;
        fwd_EX_DM_rt_toEX = 0;
        fwd_EX_DM_rs_toEX = 0;
        fwd_DM_WB_rt_toEX = 0;
        fwd_DM_WB_rs_toEX = 0;
	if(i >= 2 && comm[i].nop == 0){
	
	int reg_rt = reg[comm[i].rt];
	int reg_rs = reg[comm[i].rs];
	////////////////forward///////////////////////////
	//EX is rType(except jr)
	if((comm[i].opcode == 0) && (comm[i].func != 8)){
		//EX is rType and have both rs&rt
		if((comm[i].func >= 32 && comm[i].func <= 34) || comm[i].func == 24 || comm[i].func == 25 || (comm[i].func >= 36 && comm[i].func <= 40) || comm[i].func == 42){
			//if DM is rType
			if(comm[inst_3].opcode == 0){ 
				//no jr and mult multu
				if(comm[inst_3].func != 8  && comm[inst_3].func != 24 && comm[inst_3].func != 25){
					if(comm[inst_3].rd == comm[i].rt && comm[i].rt != 0){
						fwd_EX_DM_rt_toEX = 1;
						reg_rt = EX_DM;
					}
					if(comm[inst_3].rd == comm[i].rs && comm[i].rs != 0){
						fwd_EX_DM_rs_toEX = 1;
						reg_rs = EX_DM;
					}
				}
			}
			//if DM is iType, no load and store words
			else if(comm[inst_3].ALUSrc == 1 && comm[inst_3].RegWrite == 1 && comm[inst_3].MemRead == 0){
				if((comm[inst_3].rt == comm[i].rt) && comm[i].rt != 0){
					fwd_EX_DM_rt_toEX = 1;
	                                reg_rt = EX_DM;
				}
				if((comm[inst_3].rt == comm[i].rs) && comm[i].rs != 0){
					fwd_EX_DM_rs_toEX = 1;
		                         reg_rs = EX_DM;
				}
			}
			//if DM is jal
			else if(comm[inst_3].opcode == 3){
                                if(comm[i].rt == 31){
                                	fwd_EX_DM_rt_toID = 1;
                                	reg_rt = EX_DM;
				}
				if(comm[i].rs == 31){
					fwd_EX_DM_rs_toID = 1;
                                        reg_rs = EX_DM;
				}
                        }

			//if WB is rType
			if(comm[inst_4].opcode == 0){
				
				//no jr and mult multu
                                if(comm[inst_4].func != 8  && comm[inst_4].func != 24 && comm[inst_4].func != 25){
                                        if(comm[inst_4].rd == comm[i].rt && comm[i].rt != 0 && fwd_EX_DM_rt_toEX == 0){
                                                fwd_DM_WB_rt_toEX = 1;
                                                reg_rt = buff[comm[i].rt];
                                        }
                                        if(comm[inst_4].rd == comm[i].rs && comm[i].rs != 0 && fwd_EX_DM_rs_toEX == 0){
                                                fwd_DM_WB_rs_toEX = 1;
						reg_rs = buff[comm[i].rs];
                                        }
                                }
			}
			//if WB is iType, no store words
                        else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].RegWrite == 1){
                                if(comm[inst_4].rt == comm[i].rt && comm[i].rt != 0 && fwd_EX_DM_rt_toEX == 0){
                                        fwd_DM_WB_rt_toEX = 1;
                                        reg_rt = buff[comm[i].rt];
				}
                                if(comm[inst_4].rt == comm[i].rs && comm[i].rs != 0 && fwd_EX_DM_rs_toEX == 0){
                                        fwd_DM_WB_rs_toEX = 1;
                                        reg_rs = buff[comm[i].rs];
				}
                        }
			//if WB is jal
			else if(comm[inst_4].opcode == 3){
				if(comm[i].rt == 31){
					fwd_DM_WB_rt_toEX = 1;
                                        reg_rt = buff[31];
				}
				if(comm[i].rs == 31){
					fwd_DM_WB_rs_toEX = 1;
                                        reg_rs = buff[31];
				}
			}
		}
		//EX is rType and have only rt
		else if(comm[i].func == 0 || comm[i].func == 2 || comm[i].func == 3){
			//if DM is rType
                        if(comm[inst_3].opcode == 0){
                                //no jr and mult multu
                                if(comm[inst_3].func != 8  && comm[inst_3].func != 24 && comm[inst_3].func != 25){
                                        if(comm[inst_3].rd == comm[i].rt && comm[i].rt != 0){
                                                fwd_EX_DM_rt_toEX = 1;
                                                reg_rt = EX_DM;
                                        }
                                }
                        }
                        //if DM is iType, no load and store words
                        else if(comm[inst_3].ALUSrc == 1 && comm[inst_3].RegWrite == 1 && comm[inst_3].MemRead == 0){
                                if((comm[inst_3].rt == comm[i].rt) && comm[i].rt != 0){
                                        fwd_EX_DM_rt_toEX = 1;
                                	reg_rt = EX_DM;
                                }
                        }
			//if DM is jal
			else if(comm[i].opcode == 3){
				if(comm[i].rt == 31){
					fwd_EX_DM_rt_toEX = 1;
                                        reg_rt = EX_DM;
				}
			}
			//if WB is rType
                        if(comm[inst_4].opcode == 0){
                                //no jr and mult multu
                                if(comm[inst_4].func != 8  && comm[inst_4].func != 24 && comm[inst_4].func != 25){
                                        if(comm[inst_4].rd == comm[i].rt && comm[i].rt != 0 && fwd_EX_DM_rt_toEX == 0){
                                                fwd_DM_WB_rt_toEX = 1;
                                                reg_rt = buff[comm[i].rt];
                                        }
                                }
                        }
                        //if WB is iType, no store words
                        else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].RegWrite == 1){
                                if(comm[inst_4].rt == comm[i].rt && comm[i].rt != 0 && fwd_EX_DM_rt_toEX == 0){
                                        fwd_DM_WB_rt_toEX = 1;
                                        reg_rt = buff[comm[i].rt];
                                }
                        }
			//if WB is jal
			else if(comm[inst_4].opcode == 3){
				if(comm[i].rt == 31){
					fwd_DM_WB_rt_toEX = 1;
                                        reg_rt = buff[31];
				}
			}

		}
		switch(comm[i].func){
			case 32: //add
				EX_reg = reg_rs + reg_rt;
				if((reg_rs >= 0 && reg_rt >=0 && EX_reg < 0) || (reg_rs < 0 && reg_rt < 0 && EX_reg >= 0)){
	 				print_numoverflow(cycle);
				}

			break;

			case 33: //addu
				EX_reg = (unsigned int)reg_rs +(unsigned int)reg_rt;
			break;

			case 34: //sub
				EX_reg = reg_rs - reg_rt;
				if((reg_rs < 0 && -reg_rt < 0 && EX_reg >= 0) || (reg_rs >= 0 && -reg_rt >= 0 && EX_reg < 0)) print_numoverflow(cycle);
				
			break;
			
			case 36: //and
				EX_reg = reg_rs & reg_rt;
			break;
		
			case 37: //or
				EX_reg = reg_rs | reg_rt;
			break;

			case 38: //Xor
				EX_reg = reg_rs ^ reg_rt;
			break;

			case 39: //nor
				EX_reg = ~(reg_rs | reg_rt);
			break;

			case 40: //nand
				EX_reg = ~(reg_rs & reg_rt);
			break;

			case 42: //slt
				EX_reg = (reg_rs < reg_rt);
			break;

			case 0: //sll
				//printf("sll 0x%08X << %d = $%d: 0x%08X\n",reg_rt,comm[i].shamt,comm[i].rd,EX_DM);
				EX_reg = reg_rt << comm[i].shamt;
			break;

			case 2: //srl
				EX_reg = (unsigned int)reg_rt >> comm[i].shamt;
			break;

			case 3: //sra
				EX_reg = reg_rt >> comm[i].shamt;
			break;

			case 24: //mult
				{
				long long int rs = reg_rs;
	    			long long int rt = reg_rt;
				result = rs * rt;
				if(mf != 0)	print_overwrite(cycle);
				HI_reg = result >> 32;
            			LO_reg = (result << 32) >> 32;
				mf = 1;}
			break;

			case 25: //multu
				{
				unsigned long long int reg1 = (unsigned)reg_rs;
	    			unsigned long long int reg2 = (unsigned)reg_rt;
            			unsigned long long int mult = reg1 * reg2;
				if(mf != 0)	print_overwrite(cycle);
	    			HI_reg = mult >> 32;
	    			LO_reg = (mult << 32) >> 32;
				mf = 1;}
			break;
			
			case 16: //mfhi
				EX_reg = HI;
				mf = 0;
			break;
	
			case 18: //mflo
				EX_reg = LO;
				mf = 0;
			break;
		}
	}//end rType

	//////////////iType//////////
	//EX is addi,lw,lh,lhu,lb,lbu,sw,sh,sb
	else if(comm[i].opcode == 8 || comm[i].opcode == 35 || comm[i].opcode == 33 || comm[i].opcode == 37 || comm[i].opcode == 32 || comm[i].opcode == 36 || comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43){
		
		////////////////forward//////////////////////////
		//if DM is rType
		if(comm[inst_3].opcode == 0){
                                //no jr and mult multu
                	if(comm[inst_3].func != 8  && comm[inst_3].func != 24 && comm[inst_3].func != 25){
				if(comm[i].rs == comm[inst_3].rd && comm[i].rs != 0){
					fwd_EX_DM_rs_toEX = 1;
                                        reg_rs = EX_DM;
				}
				//for store word!(also have rt to use)
				if(comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43){
					if(comm[i].rt == comm[inst_3].rd && comm[i].rt != 0){
						fwd_EX_DM_rt_toEX = 1;
						special = EX_DM;
					}
				}
			}
		}
		//if DM is iType no store,load words and branch
		else if(comm[inst_3].ALUSrc == 1 && comm[inst_3].RegWrite == 1 && comm[inst_3].MemRead == 0){
			if(comm[inst_3].rt == comm[i].rs && comm[i].rs != 0){
				fwd_EX_DM_rs_toEX = 1;
				reg_rs = EX_DM;
			}
			//for store word!(also have rt to use)
                        if(comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43){
                                if(comm[i].rt == comm[inst_3].rt && comm[i].rt != 0){
                                	fwd_EX_DM_rt_toEX = 1;
                                        special = EX_DM;
                                }
                        }
		}
		//if DM is jal
		else if(comm[inst_3].opcode == 3){
			if(comm[i].rs == 31){
				fwd_EX_DM_rs_toEX = 1;
                                reg_rs = EX_DM;
			}
			//for store word!(also have rt to use)
                        if(comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43){
                                if(comm[i].rt == 31){
                                	fwd_EX_DM_rt_toEX = 1;
                                        special = EX_DM;
                                }
                        }
		}
		//if WB is rType
                if(comm[inst_4].opcode == 0){
                        //no jr and mult multu
                        if(comm[inst_4].func != 8  && comm[inst_4].func != 24 && comm[inst_4].func != 25){
                               if(comm[inst_4].rd == comm[i].rs && comm[i].rs != 0 && fwd_EX_DM_rs_toEX == 0){
                                        fwd_DM_WB_rs_toEX = 1;
					reg_rs = buff[comm[i].rs];
                               }
                       }
			//for store word!(also have rt to use)
			if(comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43){
                                if(comm[i].rt == comm[inst_4].rd && comm[i].rt != 0 && fwd_EX_DM_rt_toEX == 0){
                                        fwd_DM_WB_rt_toEX = 1;
                                        special = buff[comm[i].rt];
                                }
                        }
               }
               //if WB is iType, no store words
               else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].RegWrite == 1){
			if(comm[inst_4].rt == comm[i].rs && comm[i].rs != 0 && fwd_EX_DM_rs_toEX == 0){
                              fwd_DM_WB_rs_toEX = 1;
			      reg_rs = buff[comm[i].rs];
                       }
			//for store word!(also have rt to use)
                        if(comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43){
				if(comm[i].rt == comm[inst_4].rt && comm[i].rt != 0 && fwd_EX_DM_rt_toEX == 0){
                                        fwd_DM_WB_rt_toEX = 1;
                                        special = buff[comm[i].rt];
                                }
                        }
               }
		//if WB is jal
		else if(comm[inst_4].opcode == 3){
			if(comm[i].rs == 31){
				fwd_DM_WB_rs_toEX = 1;
                                reg_rs = buff[31];
			}
			//for store word!(also have rt to use)
                        if(comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43){
                                if(comm[i].rt == 31){
                                        fwd_DM_WB_rt_toEX = 1;
                                        special = buff[31];
                                }
                        }
		}

		if((comm[i].C>>15)==1)
               		comm[i].C |= 0xffff0000;
       		else
              		comm[i].C &= 0x0000ffff;			

		EX_reg = comm[i].C + reg_rs;
		//printf("reg_rs 0x%08X + C 0x%08X = 0x%08X\n",reg_rs,comm[i].C,EX_DM);
		if(fwd_DM_WB_rt_toEX == 1 && (comm[i].opcode == 40 || comm[i].opcode == 41 || comm[i].opcode == 43)){
			if((special >= 0 && comm[i].C >= 0 && EX_reg < 0) || (special < 0 && comm[i].C < 0 && EX_reg >= 0))
			print_numoverflow(cycle);
		}
		else if((reg_rs >= 0 && comm[i].C >= 0 && EX_reg < 0) || (reg_rs < 0 && comm[i].C < 0 && EX_reg >= 0)){
			print_numoverflow(cycle);			
		}
	}
	//addiu andi ori nori slti
	else if(comm[i].opcode == 9 || comm[i].opcode == 12 || comm[i].opcode == 13 || comm[i].opcode == 14 || comm[i].opcode == 10){

		////////////////forward//////////////////////////
                //if DM is rType
                if(comm[inst_3].opcode == 0){
                                //no jr and mult multu
                        if(comm[inst_3].func != 8  && comm[inst_3].func != 24 && comm[inst_3].func != 25){
                                if(comm[i].rs == comm[inst_3].rd && comm[i].rs != 0){
                                        fwd_EX_DM_rs_toEX = 1;
                                        reg_rs = EX_DM;
                                }
                        }
                }
                //if DM is iType no store,load words and branch
                else if(comm[inst_3].ALUSrc == 1 && comm[inst_3].RegWrite == 1 && comm[inst_3].MemRead == 0){
                        if(comm[inst_3].rt == comm[i].rs && comm[i].rs != 0){
                                fwd_EX_DM_rs_toEX = 1;
                                reg_rs = EX_DM;
                        }
                }
		//if DM is jal
                else if(comm[inst_3].opcode == 3){
                        if(comm[i].rs == 31){
                                fwd_EX_DM_rs_toEX = 1;
                                reg_rs = EX_DM;
                        }
                }

		//if WB is rType
                if(comm[inst_4].opcode == 0){
                        //no jr and mult multu
                        if(comm[inst_4].func != 8  && comm[inst_4].func != 24 && comm[inst_4].func != 25){ 
                               if(comm[inst_4].rd == comm[i].rs && comm[i].rs != 0 && fwd_EX_DM_rs_toEX == 0){
                                        fwd_DM_WB_rs_toEX = 1;
                                        reg_rs = buff[comm[i].rs];
                               }
                       }
               }
               //if WB is iType, no store words
               else if(comm[inst_4].ALUSrc == 1 && comm[inst_4].RegWrite == 1){
                       if(comm[inst_4].rt == comm[i].rs && comm[i].rs != 0 && fwd_EX_DM_rs_toEX == 0){
                               fwd_DM_WB_rs_toEX = 1;
                               reg_rs = buff[comm[i].rs];
                       }
               }
		//if DM is jal
                else if(comm[inst_4].opcode == 3){
                        if(comm[i].rs == 31){
                                fwd_DM_WB_rs_toEX = 1;
                                reg_rs = buff[31];
                        }
                }

		//addiu
		if(comm[i].opcode == 9){
			if((comm[i].C>>15)==1)
                        	comm[i].C |= 0xffff0000;
                	else
                        	comm[i].C &= 0x0000ffff;
			EX_reg = (unsigned int)comm[i].C + (unsigned int)reg_rs;
		}
		//andi
		else if(comm[i].opcode == 12){
			comm[i].C &= 0x0000ffff;
			EX_reg = comm[i].C & reg_rs;
		}
		//ori
		else if(comm[i].opcode == 13){
			comm[i].C &= 0x0000ffff;
                	EX_reg = comm[i].C | reg_rs;
		}
		//nori
		else if(comm[i].opcode == 14){
			comm[i].C &= 0x0000ffff;
                	EX_reg = ~(comm[i].C | reg_rs);
		}
		//slti
		else if(comm[i].opcode == 10){
			if((comm[i].C>>15)==1)
                        	comm[i].C |= 0xffff0000;
                	else
                        	comm[i].C &= 0x0000ffff;
			EX_reg = (reg_rs < comm[i].C);
		}
	}
	//lui
	else if(comm[i].opcode == 15){
                EX_reg = comm[i].C << 16;
	}

	else if(comm[i].opcode == 4 || comm[i].opcode == 5 || comm[i].opcode == 7){
		//beq
		if(comm[i].opcode == 4){
			if((comm[i].C>>15)==1)
                        	comm[i].C |= 0xffff0000;
                	else
                        	comm[i].C &= 0x0000ffff;
			if((PC < 0 && comm[i].C < 0 && PC+comm[i].C >= 0) || (PC >= 0 && comm[i].C >= 0 && PC+comm[i].C < 0))
				print_numoverflow(cycle);
		}
		//bne
		else if(comm[i].opcode == 5){
			if((comm[i].C>>15)==1)
                        	comm[i].C |= 0xffff0000;
                	else
                        	comm[i].C &= 0x0000ffff;
			if((PC < 0 && comm[i].C < 0 && PC+comm[i].C >= 0) || (PC >= 0 && comm[i].C >= 0 && PC+comm[i].C < 0))
                        	print_numoverflow(cycle);
		}
		//bgtz
		else if(comm[i].opcode == 7){
			if((comm[i].C>>15)==1)
                        	comm[i].C |= 0xffff0000;
                	else
                        	comm[i].C &= 0x0000ffff;
			if((PC < 0 && comm[i].C < 0 && PC+comm[i].C >= 0) || (PC >= 0 && comm[i].C >= 0 && PC+comm[i].C < 0))
                        	print_numoverflow(cycle);
		}
	}
	//jr
	else if(comm[i].opcode == 3){
		EX_reg = ID_EX;
	}
	EX_DM = EX_reg;
}
}
