.PHONY: clean

build: allocator

allocator: tema4.c
	gcc -o allocator tema4.c -Wall
	
clean: 
	rm -f *.o allocator
