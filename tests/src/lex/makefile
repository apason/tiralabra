CC=       gcc
STD=      _GNU_SOURCE_
OBJS=     main.o
INCLUDE=  -I "../../../src/"
LEX=	   ../../../src/lex.o
CFLAGS=   -Wall -D$(STD) $(INCLUDE) -c
TARGET=   ../../target/

.c.o:
	$(CC) $(CFLAGS) $<

all:	lex_test

lex_test:	$(OBJS)
	$(CC) $(OBJS) $(LEX)  -o $(TARGET)lex_test

clean:
	rm *.o

clobber:	clean
	rm $(TARGET)lex_test
