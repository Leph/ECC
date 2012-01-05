LEX=lex
YACC=yacc
CFLAGS=-Wall
CC=gcc

all:frontend

frontend:frontend.c scanner_frontend.c
	$(CC) $(CFLAGS) -o $@ $^

backend:backend.c scanner_backend.c
	$(CC) $(CFLAGS) -o $@ $^

frontend.c:frontend.y
	$(YACC) -o $@ --defines=frontend.tab.h $^

backend.c:backend.y
	$(YACC) -o $@ --defines=backend.tab.h $^

%.c:%.l
	$(LEX) -o $@ $^

clean:
	rm -f frontend.c backend.c
