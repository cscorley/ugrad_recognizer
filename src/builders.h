/*
 *      builders.h
 *
 * This file contains several utility functions required by the scanner which
 * will help build objects required elsewhere.
 *
 */

#ifndef builders_h
#define builders_h

#include "tokens.h"
#include "line.h"

int buildToken(char *, line *);
int buildInt(char *, line *);
void buildError(superToken *, int);
void buildSuper(superToken *, char *, int);

#endif
