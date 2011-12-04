/*
 *      util.c
 *
 * This file contains several utility functions required by the scanner which will help verity and
 * modify characters and lines.
 *
 */

#include <ctype.h>

#include "config.h"
#include "line.h"
#include "util.h"

/*
 *
 * name: up
 *
 * Converts the given string to upper case.
 *
 * @param	string	the string to be converted
 */
void up(char * string){
	int i;
	for(i=0;i<MAX_LINE_LEN;i++){
		string[i] = toupper(string[i]);
	}
}

/*
 *
 * name: isvariable
 *
 * Checks the given word if it follows the valid variable format.
 *
 * @param	word	the string to be checked
 * @return	true if the string is valid, false otherwise.
 */
int isvariable(char* word){
	int i;
	if(!isalpha(word[0])){
		return 0;
	}
	for(i=1; i < MAX_TOKEN_LEN; i++){
		if(word[i] == '\0'){
			return 1;
		}
		if(!isalnum(word[i])){
			return 0;
		}
	}
	return 1;
}

/*
 *
 * name: isinteger
 *
 * Checks the given word if it follows the valid integer format.
 *
 * @param	word	the string to be checked
 * @return	true if the string is valid, false otherwise.
 */
int isinteger(char* word){
	int i;
	if(word[0] != '-' && word[0] != '+' && !isdigit(word[0])){
		return 0;
	}
	for(i=1; i< MAX_TOKEN_LEN; i++){
		if(word[i] == '\0'){
			return 1;
		}
		if(!isdigit(word[i])){
			return 0;
		}
	}
	return 1;
}

/*
 *
 * name: isstopper
 *
 * Checks if the given character is a stopper.
 *
 * @param	stuff	the character to check
 * @return	true if the character is a stopper, false otherwise.
 */
int isstopper(char character){
	if(isspace(character)){
		return 1;
	}
	switch(character){
		case ':':
		case ';':
		case '(':
		case ')':
		case ',':
		case '+':
		case '-':
		case '*':
			return 1;
	}
	return 0;
}

/*
 *
 * name: skipWhitespace
 *
 * Skips all of the whitespace from the scanIndex in the given line.
 * Will set scanIndex at the first character from its previous location.
 *
 * @param	current	the line for whitespace to be skipped
 */
void skipWhitespace(line * current){
	int i;
	// will hopefully skip all whitespace on line
	for(i=current->scanIndex; i < MAX_LINE_LEN; i++){
		if(!isspace(current->line[i])){
			current->scanIndex=i;
			return;
		}
	}
}

/*
 *
 * name: skipComment
 *
 * Will skip all the text on the given line until the end of comment characters
 * are found.  It will return 0 if unsuccessful or needs a new line.
 *
 * @param	current	the line to have comments skipped on
 * @return	0 if unfinished, 1 if end of comment found
 */
int skipComment(line * current){
	int i = current->scanIndex;
	if(current->line[i] == '\0' && !current->atEOF){
		return 0;
	}
	for(;i < MAX_LINE_LEN; i++){
		if(current->line[i] == '*' &&
			current->line[i+1] == ')'){
			current->scanIndex = i + 2;
			return 1;
		}
	}
	return 0;
}
