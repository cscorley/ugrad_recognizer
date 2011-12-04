/*
 *      scanner.h
 *
 * This file contains the main scanner function, getToken.  All scanning can be controlled through this
 * function.
 */

#ifndef scanner_h
#define scanner_h

#include "tokens.h"
#include "line.h"

void getLine(line *, FILE* );
superToken getToken(line *, FILE*, token **);

#endif
