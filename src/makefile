CC=	gcc
STD=	_GNU_SOURCE_
OBJS=	main.o lex.o parser.o semantics.o memory.o
CFLAGS=	-Wall -D$(STD) -c -Werror -g
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
