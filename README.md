This was written for a undergraduate course in 2010. Still searching for
original documentation/requirements.

* To compile the parser using the given makefile, run:
    `make all`
* If the make utility is not included on your system, you may compile each file manually:
    `gcc -c hasher.c`
    `gcc -c util.c`
    `gcc -c builders.c`
    `gcc -c scanner.c`
    `gcc -c grammar.c`
    `gcc -c parser.c`
    `gcc hasher.o util.o builders.o scanner.o grammar.c parser.o -o parser`
* Either of these steps will generate the executable file named "parser"
* To execute the parser, you can either pass the test file name directly as a parameter:
    `./parser test`
* Or simply run the parser and it will ask you for a file name on execution:
    `./parser`
* When the parser executes, it will give a full print out of the source code, if there was a successful parse, and the symbol table.  Errors will be included when one is encountered and then the next token is found.
