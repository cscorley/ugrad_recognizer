/*
 *      hasher.h
 *
 */

#ifndef hasher_h
#define hasher_h

#include "tokens.h"

void readTokens(token *, char *);
int hash(char *);
int getHash(token **, char *);
int insertHash(token **, char *, int);
void buildHashes(token **, token *);
void printHash(token **);

#endif
