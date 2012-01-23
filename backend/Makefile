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
	rm -f backend.c scanner_backend.c lex.yy.c
	rm -f backend.tab.h backend.tab.c backend backend_test backend_test.s

test: backend
	./backend backend_test.v2a

run: backend
	./backend backend_test.v2a > backend_test.s
	gcc -Wall backend_test.s -o backend_test -g -ggdb
	./backend_test

.PHONY: test run clean

