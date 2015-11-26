CC=	gcc
STD=	_GNU_SOURCE_
OBJS=	main.o lex.o parser.o semantics.c
CFLAGS=	-Wall -D$(STD) -c -Werror
TARGET= ../target/

.c.o:
	$(CC) $(CFLAGS) $<

all:	project

project:	$(OBJS)
		$(CC) $(OBJS) -o $(TARGET)tira

clean:
	rm -f *.o

clobber:	clean
	rm $(TARGET)tira
