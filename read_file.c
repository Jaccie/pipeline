#include <stdio.h>
#include "struct.h"

command comm[258];
data d[258];
int reg[32];
int Hi,Lo;
FILE *snapshot, *error_dump;
int num_of_instruction;
void read_file(){
    snapshot = fopen("snapshot.rpt","w+t");
    error_dump = fopen("error_dump.rpt","w+t");

    FILE* iimage = fopen("iimage.bin", "rb");
    FILE* dimage = fopen("dimage.bin", "rb");
    if(!iimage) printf("cannot open the iimage file...");
    else if(!dimage) printf("cannot open the dimage file...");
    else{
        unsigned int in[258],tmp,tmp2,tmp3,tmp4,k;
        fread (&in,sizeof(unsigned int),258,iimage);
        tmp  =  in[1] << 24 ;
        tmp2 = ((in[1]  << 16) >> 24)<<16;
        tmp3 = ((in[1]  << 8) >> 24)<<8;
        tmp4 = in[1]  >> 24;
        k = tmp + tmp2 + tmp3 + tmp4 ;
        int i;
	num_of_instruction = k;
	//printf("K=%d\n",k);
        for(i = 0 ; i < k+2 ; i++){
            tmp  =  in[i] << 24 ;
            tmp2 = ((in[i]  << 16) >> 24) << 16;
            tmp3 = ((in[i]  << 8) >> 24) << 8;
            tmp4 = in[i]  >> 24;
            in[i] = tmp + tmp2 + tmp3 + tmp4 ;
            if(i==0) comm[i].address = in[i];
	    else comm[i].address = comm[0].address + 4*(i-2);
            comm[i].i = i;
            comm[i].instruction = in[i];
          // printf("i = %d , 0x%08x Address: %08x\n",i,comm[i].instruction,comm[i].address);
        }
	PC = comm[0].instruction;
	comm[0].name = "NOP";
	comm[1].name = "NOP";

	unsigned int in_d[258];
	fread(&in_d,sizeof(unsigned int),258,dimage);
	tmp = in_d[1] << 24;
	tmp2 = ((in_d[1] << 16) >> 24) << 16;
	tmp3 = ((in_d[1] << 8) >> 24) << 8;
	tmp4 = in_d[1] >> 24;
	k = tmp + tmp2 + tmp3 + tmp4;
	for(i = 0 ; i < k+2 ; i++){
		tmp = in_d[i] << 24;
		tmp2 = ((in_d[i]  << 16) >> 24) << 16;
            	tmp3 = ((in_d[i]  << 8) >> 24) << 8;
            	tmp4 = in_d[i]  >> 24;
            	in_d[i] = tmp + tmp2 + tmp3 + tmp4 ;
		if(i==0) reg[29] = in_d[i];
		else d[i].address = i*4;
		d[i].value = in_d[i];
		//printf("i = %d , 0x%08x Address: %08x\n",i,d[i].value,d[i].address);
	}
   }
}


