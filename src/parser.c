/*
 *      parser.c
 *
 * This program parses a file in SPS and validates the source code.
 *
 * Input: A file containing a program source written in SPS
 *
 * Output: The program source with any errors and if the parse was successful 
 * 	and the symbol table.
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
#include "parser.h"
#include "grammar.h"


/*
 *
 * name: main
 *
 * Receives information from the user of the input and output files, and then
 * makes appropriate calls.
 *
 * @param	argc	the number of arguments passed (including the program)
 * @param	argv	the argument array of the program call
 * @return	error code
 */
int main(int argc, char** argv){
	line current;
	sourceContainer source;
	source.current = &current;

	char input[MAX_FILE_LEN];
	int i;
	token tokenList[MAX_TOKENS];

	// The user can pass a parameter to the program for the file name.
	// If no parameter is given, the program will ask explicitly.
	if(argc == 2){
		strcpy(input, argv[1]);
	}
	else{
		printf("\n Name of your input file (%d characters max): ", MAX_FILE_LEN);
		scanf("%s", input);
	}
	source.infile = fopen(input, "r");

	if(source.infile == NULL){
		printf("Could not open input file!\n");
		exit(1);
	}

	// prepare and build the token table
	for(i=0;i<HASH_TABLE_SIZE;i++){
		source.hashTable[i] = NULL;
		source.symbolTable[i] = NULL;
	}

	readTokens(tokenList, "tokens");
	buildHashes(source.hashTable, tokenList);

	source.current->scanIndex = 0;
	source.current->lineNumber = 0;
	source.current->atEOF = 0;
	memset(source.current->line, '\0', MAX_LINE_LEN);


	// parse the source
	if(prog(&source)){
		printf("\n\nParse successful!\n");
	}
	else{
		while(!source.current->atEOF){
			getLine(source.current, source.infile);
		}
		printf("\n\nParse failure!\n");
	}

	printf("\nSymbol table:\n");
	printHash(source.symbolTable);
	return 0;
}
