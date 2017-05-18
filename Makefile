pipeline: main.o read_file.o Halt.o pipe.o print_snapshot.o IF.o ID.o EX.o DM.o WB.o print_error_dump.o struct.h
	gcc -o pipeline main.o read_file.o Halt.o pipe.o print_snapshot.o IF.o ID.o EX.o DM.o WB.o print_error_dump.o struct.h

main.o: main.c
	gcc -g -c main.c
read_file.o: read_file.c
	gcc -g -c read_file.c
pipe.o: pipe.c
	gcc -g -c pipe.c
print_snapshot.o: print_snapshot.c
	gcc -g -c print_snapshot.c
IF.o: IF.c
	gcc -g -c IF.c
ID.o: ID.c
	gcc -g -c ID.c
EX.o: EX.c
	gcc -g -c EX.c
DM.o: DM.c
	gcc -g -c DM.c
WB.o: WB.c
	gcc -g -c WB.c
print_error_dump.o: print_error_dump.c
	gcc -g -c print_error_dump.c
Halt.o: Halt.c
	gcc -g -c Halt.c
clean:
	rm -f pipeline main.o read_file.o Halt.o pipe.o print_snapshot.o IF.o ID.o EX.o DM.o WB.o print_error_dump.o
