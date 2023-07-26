all: test

CC = g++-10
OPT= -std=c++2a -g -flto  
CFLAGS = $(OPT) -Wall -march=native 
LIBS = -lssl -lcrypto 

veb: vebtest.cc vebTree.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

bench: bench.cc vebTree.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f veb bench 