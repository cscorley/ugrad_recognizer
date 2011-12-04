OBJS = hasher.o util.o builders.o scanner.o grammar.o parser.o
CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

all : parser

parser : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o parser

hasher.o : config.h tokens.h hasher.h
	$(CC) $(CFLAGS) hasher.c

util.o : config.h tokens.h line.h util.h 
	$(CC) $(CFLAGS) util.c

builders.o : config.h tokens.h line.h util.h builders.h
	$(CC) $(CFLAGS) builders.c

scanner.o : config.h tokens.h line.h hasher.h util.h builders.h scanner.h
	$(CC) $(CFLAGS) scanner.c

grammar.o : config.h tokens.h line.h hasher.h util.h builders.h scanner.h grammar.h
	$(CC) $(CFLAGS) grammar.c
	
parser.o : config.h tokens.h line.h hasher.h util.h builders.h scanner.h grammar.h parser.h
	$(CC) $(CFLAGS) parser.c

clean:
	\rm -f *.o parser

srctar:
	tar cjvf cscorley_src.tar.bz2 *.h *.c makefile

tar: parser
	tar cjvf cscorley_build.tar.bz2 parser
