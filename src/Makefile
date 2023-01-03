all: testDIR.out testBB.out testINIT.out

test%.out: test%.c bitboard.o
	cc $^ -o $@

clean:
	rm -rf *.o *.out
