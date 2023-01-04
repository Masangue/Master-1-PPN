CC=gcc
LDFLAGS=-lm

all: image_processing

image_processing: src/image_processing.c
	$(CC) -o a.out $< -I./lib/ -L./lib/ -lspng 

clean:
	rm -f main image_processing *.o *.out

.PHONY: clean all
