CFLAGS=-O3 -Wall -Wno-unused-function

all: prng prng.h
	./prng

rand: clean
rand: CFLAGS+=-DUSE_RAND
rand: all

custom: clean
custom: all

clean:
	rm -f prng
