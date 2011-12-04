/*
 *      scanner.c
 *
 * This file contains the main scanner function, getToken.  All scanning can
 * be controlled through this function.  Our driver is also located within
 * this file.
 *
 * Input: A file containing a program source written in SPS
 *
 * Output: The scanned token's and associated identifiers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "tokens.h"
#include "line.h"
#include "hasher.h"
#include "util.h"
#include "builders.h"
#include "scanner.h"

/*
 *
 * name: getLine
 *
 * Will read into the given line from input a new string.
 *
 * @param	current	the line to be read into
 * @param	input	the file handler to be read from
 */
void getLine(line * current, FILE* input){
	if(!feof(input)){
		memset(current->line, '\0', MAX_LINE_LEN);
		fgets(current->line, MAX_LINE_LEN+1, input);
		current->scanIndex = 0;
		current->lineNumber++;

		if(current->line[0] != '\0'){
			printf("\n%d\t: %s", current->lineNumber, current->line);
		}

		up(current->line);
		if(feof(input)){
			current->atEOF = 1;
			return;
		}
	}
	else{
		current->atEOF = 1;
		return;
	}
}

/*
 *
 * name: getToken
 *
 * The main function of the scanner.  It will call all the other functions
 * as needed.  This will handle all the work of the scanner.
 *
 * @param	current	the line to be read into
 * @param	input	the file to be read from
 * @param	hashTable	the lookup table for tokens
 * @return	a wrapper token which includes the token and error information
 */
superToken getToken(line * current,FILE* input,token ** hashTable){
	superToken toReturn;
	int i, err, hashVal;
	char word[9];

	toReturn.error = 0;
	memset(word, '\0', MAX_TOKEN_LEN+1);
	memset(toReturn.message, '\0', MAX_MESSAGE_LEN);
	while(word[0] == '\0' && !current->atEOF){
		i= current->scanIndex;
		if(current->line[i] == '\0'){
			if(!current->atEOF){
				getLine(current, input);
			}
			else{
				toReturn.error = -1;
				return toReturn;
			}
		}
		err = buildToken(word, current);

		// on 1 an integer was detected and we should verify
		if(err == 1){
			if(!isinteger(word)){
				buildError(&toReturn, 3);
			}
			buildSuper(&toReturn, word, 23);
			return toReturn;
		}

		// on 2 a comment was detected so we should skip and try to get another token
		while(err == 2){
			//comment mode on, ignore until "*)" is found
			while(!skipComment(current) && !current->atEOF){
				getLine(current, input);
			}
			err = buildToken(word, current);
		}
	}

	hashVal = getHash(hashTable, word);

	// on -1 from getHash the word was not in the table, so it must be a variable
	if(hashVal == -1){
		if(!isvariable(word)){
			buildError(&toReturn, 2);
		}

		// on -1 from buildToken then the variable's length was too long.
		if(err == -1){
			buildError(&toReturn, 1);
			// skip the remainder of the offending token
			while(!isstopper(current->line[current->scanIndex])){
					current->scanIndex++;
			}
		}
		buildSuper(&toReturn, word, 22);
	}
	else{
		toReturn.item = *hashTable[hashVal];
	}
	return toReturn;
}
