/*
 *      grammar.h
 *
 * These functions are the main BNF rules for the recursive descent parser.
 * There are extra utility functions also packaged here.
 *
 */

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "line.h"
#include "tokens.h"

typedef struct {
	superToken currentToken;
	FILE* infile;
	token* hashTable[HASH_TABLE_SIZE];
	token* symbolTable[HASH_TABLE_SIZE];
	line * current;
} sourceContainer;

void err(char *, superToken);
int addId(sourceContainer*, int);
int lookupId(sourceContainer*);

int prog(sourceContainer*);
int progName(sourceContainer*);
int decList(sourceContainer*);
int dec(sourceContainer*);
int type(sourceContainer*);
int idList(sourceContainer*, int);
int stmtList(sourceContainer*);
int stmt(sourceContainer*);
int assign(sourceContainer*);
int expression(sourceContainer*);
int term(sourceContainer*);
int factor(sourceContainer*);
int read(sourceContainer*);
int write(sourceContainer*);
int forStmt(sourceContainer*);
int indexExp(sourceContainer*);
int body(sourceContainer*);

#endif
