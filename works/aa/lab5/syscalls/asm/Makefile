.PHONY: all

all:
	nasm -f elf64 hello64.asm
	gcc -m64 -o hello_64 hello64.o

clean:
	rm *.o
	rm nasm_hello hello_64