LEX=lex
YACC=yacc
CFLAGS=-Wall -g -ggdb
CC=gcc

all:backend

backend:backend.c scanner_backend.c ecc.c asm.c
	$(CC) $(CFLAGS) -o $@ $^

backend.c:backend.y
	$(YACC) -o $@ --defines=backend.tab.h $^

%.c:%.l
	$(LEX) -o $@ $^

clean:
	rm -f backend.c scanner_backend.c
	rm -f backend.tab.h backend

test: backend
	./backend test.v2c

run: backend
	./backend test.v2c > test.s
	gcc -Wall test.s -o test -g -ggdb
	./test || echo ">>>" $$?

.PHONY: test run clean

