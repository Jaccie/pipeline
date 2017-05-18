#include <stdio.h>
#include "struct.h"

FILE* error_dump;

void print_writezero(int cycle){
	fprintf(error_dump,"In cycle %d: Write $0 Error\n",cycle);
	//printf("In cycle %d: Write $0 Error\n",cycle);
}

void print_numoverflow(int cycle){
	fprintf(error_dump,"In cycle %d: Number Overflow\n",cycle);
	//printf("In cycle %d: Number Overflow\n",cycle);
}

void print_overwrite(int cycle){
	fprintf(error_dump,"In cycle %d: Overwrite HI-LO registers\n",cycle);
	//printf("In cycle %d: Overwrite HI-LO registers\n",cycle);
}

void print_memoverflow(int cycle){
	fprintf(error_dump,"In cycle %d: Address Overflow\n",cycle);
	//printf("In cycle %d: Address Overflow\n",cycle);
}

void print_misaligned(int cycle){
	fprintf(error_dump,"In cycle %d: Misalignment Error\n",cycle);
	//printf("In cycle %d: Misalignment Error\n",cycle);
}
