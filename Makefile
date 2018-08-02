test: test.c hashtable.h hashfn.h
	gcc -Wall -o test test.c hashfn.c
