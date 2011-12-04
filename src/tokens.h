/*
 *      tokens.h
 *
 * This file contains the token structs needed throughout the program.
 *
 */

#ifndef tokens_h
#define tokens_h

#include "config.h"
#include "line.h"

// the enumeration which can be used throughout the program
enum {PROGRAM=1, VAR, BEGIN, END, ENDDOT, INTEGER, FOR, READ, WRITE, TO,
	DO, SEMICOLON, COLON, COMMA, COLONEQUALS, PLUS, MINUS, ASTRIX, DIV,
	LEFTPAREN, RIGHTPAREN, ID, INT};

typedef struct{
	char name[MAX_TOKEN_LEN+1];
	int code;
} token;

typedef struct{
	token item;
	int error;
	char message[MAX_MESSAGE_LEN];
} superToken;

#endif
