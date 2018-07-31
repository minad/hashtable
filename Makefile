test: test.c hash.h hashfn.h
	gcc -Wall -o test test.c hashfn.c
