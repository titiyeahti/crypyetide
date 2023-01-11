CFLAGS=-g -O3
LDFLAGS=-g -O3

all: testDIR.out testBB.out testINIT.out testCRYPT.out

test%.out: test%.c bitboard.o cryptide.o
	cc $^ $(LDFLAGS) -o $@ 

clean:
	rm -rf *.o *.out
