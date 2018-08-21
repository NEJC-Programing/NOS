default:
	g++ nos_sorce.cpp -c -m32 -nostdlib
	gcc nos.c -c -m32 -nostdlib
	ld -melf_i386 -r nos.o nos_sorce.o -o NOS_Lib.o
	rm nos.o nos_sorce.o
