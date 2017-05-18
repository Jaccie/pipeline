#include <stdio.h>

void read_file();
typedef struct read_iimage{
    int i;
    unsigned int instruction;
    unsigned int address;
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int shamt;
    unsigned int func;
    int C;
    unsigned int jump;

    int halt;
    char* name;
    int RegWrite;
    int Branch;
    int RegDst;
    int MemRead;
    int MemtoReg;
    int MemWrite;
    int ALUSrc;
    int nop;
}command;

typedef struct read_dimage{
	unsigned int address;
	int i;
	unsigned int value;
}data;

extern int EX_reg;
extern int special;
extern int HI,LO;
extern int HI_reg,LO_reg;
extern int mf;
extern int cycle;
extern command comm[258];
extern data d[258];
extern int reg[32];
extern int buff[32];
extern unsigned int PC;
extern FILE *snapshot, *error_dump;
extern int isHalt;
extern int num_of_instruction;

extern int fwd_EX_DM_rt_toID;
extern int fwd_EX_DM_rs_toID;
extern int fwd_EX_DM_rt_toEX;
extern int fwd_EX_DM_rs_toEX;
extern int fwd_DM_WB_rt_toEX;
extern int fwd_DM_WB_rs_toEX;
extern int ID_Stall;
extern int flush;

extern int inst_next;
extern int inst_0;
extern int inst_1;
extern int inst_2;
extern int inst_3;
extern int inst_4;
extern int ID_EX;
extern int EX_DM;
extern int DM_WB;

void pipe();
void IF();
void ID(int i);
void EX(int i);
void DM(int i);
void WB(int i);
void halt();

void print_cyc(int cycle);
void print_reg();
void print_pc(unsigned int PC);
void print_IF(int i);
void print_IFflush(int i);
void print_IFstall(int i);
void print_ID(int i);
void print_IDstall(int i);
void print_EX(int i);
void print_DM(int i);
void print_WB(int i);

void print_writezero(int cycle);
void print_numoverflow(int cycle);
void print_overwrite(int cycle);
void print_memoverflow(int cycle);
void print_misaligned(int cycle);

