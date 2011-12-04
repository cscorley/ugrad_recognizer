/*
 *      builders.c
 *
 * This file contains several utility functions required by the scanner which
 * will help build objects required elsewhere.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "config.h"
#include "tokens.h"
#include "line.h"
#include "util.h"
#include "builders.h"

/*
 *
 * name: buildToken
 *
 * Builds the next token into the given word from the current line.
 *
 * @param	word	the string to contain the read token
 * @param	current	the line to be read from
 * @return	0 on success, 1 on integer, 2 on comment, -1 on error
 */
int buildToken(char * word, line * current){
	int i;
	int wordPos = 0;
	memset(word, '\0', MAX_TOKEN_LEN+1);
	skipWhitespace(current);

	for(i=current->scanIndex; i < MAX_LINE_LEN; i++){
		wordPos = i - current->scanIndex; // current position in word
		if(wordPos > MAX_TOKEN_LEN){
			return -1;
		}

		// catch any tokens that were used as stoppers on last call
		if(wordPos==0){
			// test for special double tokens
			if(current->line[i] == '(' && current->line[i+1] == '*'){
				current->scanIndex = i + 2;
				return 2;
			}
			if(current->line[i] == ':' && current->line[i+1] == '='){
				word[wordPos] = ':';
				word[wordPos+1] = '=';
				current->scanIndex = i+2;
				return 0;
			}

			// look for integers as well
			if(isdigit(current->line[i])){
				current->scanIndex = i;
				buildInt(word, current);
				return 1;
			}

			// stoppers are tokens too!
			if(isstopper(current->line[i])){
				word[wordPos] = current->line[i];
				current->scanIndex = i+1;
				return 0;
			}
		}

		if(isstopper(current->line[i])){
			current->scanIndex = i;
			return 0;
		}
		else{
			word[wordPos] = current->line[i];
		}

		if(wordPos == MAX_TOKEN_LEN){
			return 0;
		}
	}
	current->scanIndex = i;
	return 0;
}

/*
 *
 * name: buildInt
 *
 * Builds an integer token into the given word from the current line.
 *
 * @param	word	the string to be read into
 * @param	current	the line to be read from
 * @return	0 if unsuccessful, 1 if successful
 */
int buildInt(char * word, line * current){
	int j = 0;
	int k;

	memset(word, '\0', MAX_TOKEN_LEN+1);
	word[j] = current->line[current->scanIndex];
	j++;

	for(k=current->scanIndex+1; (k < MAX_LINE_LEN) && (j < MAX_TOKEN_LEN); k++){
		if(isdigit(current->line[k]) || !isstopper(current->line[k])){
			// we will go ahead and copy invalid characters, but not stoppers.
			// the invalid characters should be caught during integer validation.
			word[j] = current->line[k];
			j++;
		}
		else{
			current->scanIndex = k;
			return 1;
		}
	}
	return 0;
}

/*
 *
 * name: buildError
 *
 * Stores into the given token the give error value and associated message.
 *
 * @param	token	the superToken to be read into
 * @param	error	the error code
 */
void buildError(superToken * super, int error){
	super->error = error;
	switch(error){
		case 0:
			strcpy(super->message, "No error.");
			break;

		case 1:
			strcpy(super->message, "Token too long.");
			break;

		case 2:
			strcpy(super->message, "Invalid variable.");
			break;

		case 3:
			strcpy(super->message, "Invalid integer.");
			break;

		default:
			strcpy(super->message, "Unknown error.");
	}
}

/*
 *
 * name: buildSuper
 *
 * A simple wrapper function to assign then given parameters to the superToken.
 *
 * @param	super	the superToken to be built
 * @param	item	the token
 * @param	word	the token name to be assigned
 * @param	code	the token code to be assigned
 */
void buildSuper(superToken * super, char * word, int code){
	token item;
	strcpy(item.name, word);
	item.code = code;
	super->item = item;
}

