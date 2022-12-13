CC=gcc
LDFLAGS=-lm

all: main image_processing

main: main.c
	$(CC) -o $@ $< $(LDFLAGS)

image_processing: image_processing.c
	$(CC) -o $@ $< -I./lib/ -L./lib/ -lspng 

clean:
	rm -f main image_processing *.o

.PHONY: clean all