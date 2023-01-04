CC=gcc
LDFLAGS=  -L./lib/ -lspng -lm
INCLUDE= -I./lib/

all: prog

prog: main image_processing nn 
	$(CC) -o prog.out build/main.o build/nn.o build/image_processing.o  $(LDFLAGS)

main: src/main.c
	$(CC) -c -o build/main.o $< $(INCLUDE)

image_processing: src/image_processing.c
	$(CC) -c -o build/image_processing.o $<  $(INCLUDE)

nn: src/nn.c
	$(CC) -c -o build/nn.o $< $(INCLUDE)

clean:
	rm -f main image_processing *.o *.out build/*

.PHONY: clean all
