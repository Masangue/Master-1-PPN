CC=gcc
LDFLAGS=  -L./lib/ -lspng -lm
INCLUDE= -I./lib/ -I./src/
CFLAGS= -g -O2 -march=native


##
all: try prog


##
prog: main image_processing nn file_manager store
	$(CC) -o prog.out build/main.o build/nn.o build/image_processing.o build/file_manager.o build/store.o $(CFLAGS) $(LDFLAGS)

try: testing image_processing nn file_manager store
	$(CC) -o test.out build/testing.o build/nn.o build/image_processing.o build/file_manager.o build/store.o $(CFLAGS) $(LDFLAGS)


##
main: src/main.c
	$(CC) -c -o build/main.o $< $(INCLUDE)  $(CFLAGS)

testing: src/testing.c
	$(CC) -c -o build/testing.o $< $(INCLUDE)  $(CFLAGS)

image_processing: src/image_processing.c
	$(CC) -c -o build/image_processing.o $<  $(INCLUDE)  $(CFLAGS)

file_manager: src/file_manager.c
	$(CC) -c -o build/file_manager.o $<  $(INCLUDE)  $(CFLAGS)

store: src/store.c
	$(CC) -c -o build/store.o $< $(INCLUDE)  $(CFLAGS)

nn: src/nn.c
	$(CC) -c -o build/nn.o $< $(INCLUDE)  $(CFLAGS)





#############         TEST          ###################
#######################################################


##
Test : Test_conv
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./test-conv.out


##
Test_conv : testconv image_processing
	$(CC) -g build/image_processing.o build/testconv.o -o test-conv.out -lcmocka $(CFLAGS) $(LDFLAGS)


##
testconv: Test/testconv.c
	$(CC) -c Test/testconv.c -o build/testconv.o -lcmocka $(INCLUDE)  $(CFLAGS)




#############         CLEAN         ###################
#######################################################



clean:
	rm -f main image_processing *.o *.out build/*

.PHONY: clean all
