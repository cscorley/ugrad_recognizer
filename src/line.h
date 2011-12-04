/*
 *      line.h
 *
 * This file contains the line struct needed throughout the program.
 *
 */

#ifndef line_h
#define line_h

#include "config.h"

typedef struct{
	char line[MAX_LINE_LEN+1];
	int scanIndex;
	int lineNumber;
	int atEOF;
} line;

#endif
