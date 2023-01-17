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
Test : Test_conv Test_Storage Test_nn
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./test-conv.out
	echo ""
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./test-storage.out
	echo ""
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./test-nn.out


##
Test_conv : testConv image_processing
	$(CC) -g build/image_processing.o build/test-conv.o -o test-conv.out -lcmocka $(CFLAGS) $(LDFLAGS)

Test_Storage : testStorage store nn
	$(CC) -g build/nn.o build/store.o build/test-storage.o -o test-storage.out -lcmocka $(CFLAGS) $(LDFLAGS)

Test_nn : testNn nn
	$(CC) -g build/nn.o build/test-nn.o -o test-nn.out -lcmocka $(CFLAGS) $(LDFLAGS)




##
testConv: Test/test-conv.c
	$(CC) -c Test/test-conv.c -o build/test-conv.o -lcmocka $(INCLUDE)  $(CFLAGS)

testStorage: Test/test-storage.c
	$(CC) -c Test/test-storage.c -o build/test-storage.o -lcmocka $(INCLUDE)  $(CFLAGS)

testNn: Test/test-nn.c
	$(CC) -c Test/test-nn.c -o build/test-nn.o -lcmocka $(INCLUDE) $(CFLAGS)






#############         CLEAN         ###################
#######################################################



clean:
	rm -f main image_processing *.o *.out build/*

.PHONY: clean all
