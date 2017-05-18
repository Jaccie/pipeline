#include <stdio.h>
#include "struct.h"

command comm[258];
int cycle;
FILE *snapshot, *error_dump;
int main(){
	snapshot = fopen("snapshot.rpt","w+r");
	error_dump = fopen("error_dump.rpt","w+r");
	read_file();
	cycle = 0;
	pipe();
	return 0;
}
