CC=gcc
LDFLAGS=-lm

all: image_processing

image_processing: image_processing.c
	$(CC) -o $@ $< -I./lib/ -L./lib/ -lspng 

clean:
	rm -f main image_processing *.o

.PHONY: clean all