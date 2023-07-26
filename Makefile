all: test

CC = g++-10
OPT= -std=c++2a -g -flto  
CFLAGS = $(OPT) -Wall -march=native -I/usr/local/stow/gcc/i386_linux22/gcc-10.2.0/include/c++/10.2.0/
LIBS = -lssl -lcrypto 

veb: vebtest.cc vebTree.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

bench: bench.cc vebTree.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f veb bench test test.o