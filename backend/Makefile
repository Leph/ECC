LEX=lex
YACC=yacc
CFLAGS=-Wall
CC=gcc

all:backend

backend:backend.c scanner_backend.c
	$(CC) $(CFLAGS) -o $@ $^

backend.c:backend.y
	$(YACC) -o $@ --defines=backend.tab.h $^

%.c:%.l
	$(LEX) -o $@ $^

clean:
	rm -f backend.c scanner_backend.c
	rm -f backend.tab.h backend
