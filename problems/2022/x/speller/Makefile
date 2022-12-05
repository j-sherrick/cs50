CC=clang
CFLAGS=-ggdb3 -gdwarf-4 -O0 -Qunused-arguments -std=c11 -Wall -Werror -Wextra -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow

speller:
	$(CC) $(CFLAGS) -c -o speller.o speller.c
	$(CC) $(CFLAGS) -c -o dictionary.o dictionary.c
	$(CC) $(CFLAGS) -o speller speller.o dictionary.o -lm

clean:
	rm speller speller.o dictionary.o