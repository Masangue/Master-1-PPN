CC=gcc
LDFLAGS=  -L./lib/ -lspng -lm
INCLUDE= -I./lib/
CFLAGS= -g -O2 -march=native

all: prog

prog: main image_processing nn file_manager
	$(CC) -o prog.out build/main.o build/nn.o build/image_processing.o build/file_manager.o $(CFLAGS) $(LDFLAGS)

main: src/main.c
	$(CC) -c -o build/main.o $< $(INCLUDE)  $(CFLAGS)

image_processing: src/image_processing.c
	$(CC) -c -o build/image_processing.o $<  $(INCLUDE)  $(CFLAGS)

file_manager: src/file_manager.c
	$(CC) -c -o build/file_manager.o $<  $(INCLUDE)  $(CFLAGS)


nn: src/nn.c
	$(CC) -c -o build/nn.o $< $(INCLUDE)  $(CFLAGS)

clean:
	rm -f main image_processing *.o *.out build/*

.PHONY: clean all
