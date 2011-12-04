/*
 *      util.h
 *
 * This file contains several utility functions required by the scanner which will help verity and
 * modify characters and lines.
 *
 */

#ifndef util_h
#define util_h

#include "line.h"

void up(char *);
int isvariable(char *);
int isinteger(char *);
int isstopper(char);
void skipWhitespace(line *);
int skipComment(line *);

#endif
