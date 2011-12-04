/*
 *      hasher.c
 *
 * A hashing program which accepts as input a text file.  The program will read
 * the text file into a table and then hash the elements of the table into a
 * hash table for later use.
 *
 * Input: A file containing the token string followed by a space followed by
 * 	the token id.
 *
 * Output: Prints the results of the hash table to the screen.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "tokens.h"
#include "hasher.h"

/*
 *
 * name: printHash
 *
 * Prints the contents of the token* array passed to it.
 *
 * @param	hashes	pointer to the array of token hashes
 */
void printHash(token ** hashes){
	int i;

	printf("\tHash\tName\tType\n");
	printf("\t----\t-----\t------------\n");

	for(i=0; i<HASH_TABLE_SIZE; i++){
		if(hashes[i] == NULL){
			printf("\t%d\n", i);
		}
		else{
			printf("\t%d\t%s\t%d\n", i, hashes[i]->name, hashes[i]->code);
		}
	}
}


/*
 *
 * name: hash
 *
 * The hashing method used to determine the position of the token in the table.
 * Uses a simple scheme of summing each character's ASCII value and considers
 * the rest of the string as being padded with spaces.
 *
 * @param	token	the token to be hashed
 * @return	position in table of token
 */
int hash(char * string){
	int i,sum=0;

	for(i=0;i<MAX_TOKEN_LEN;i++){
		if(string[i] == '\0'){
			// When we reach the end of the string read, we add
			// the remaining characters as if they were padded
			// spaces.
	  		sum = sum + (MAX_TOKEN_LEN-i)*(int)' ';
			return sum % HASH_TABLE_SIZE;
		}
		sum = sum + (int)(string[i]);
	}
	return sum % HASH_TABLE_SIZE;
}

/*
 *
 * name: getHash
 *
 * Will return the location of the given word in the given hashTable.  This
 * takes into account a possible collision and ensures the correct value is
 * matched before returning the location.
 *
 * @param	hashTable	the built hash table to look up
 * @param	word	the word to hash and search for
 * @return	the location of the word in the hash table, -1 if it DNE
 */
int getHash(token ** hashTable, char * word){
	int attemptsLeft = HASH_TABLE_SIZE;
	int hashVal = hash(word);
	if(hashTable[hashVal] == NULL){
		return -1;
	}
	else{
		while(strcmp(hashTable[hashVal]->name, word) != 0 && attemptsLeft){
			hashVal = (hashVal+1) % HASH_TABLE_SIZE;
			if(hashTable[hashVal] == NULL){
				return -1;
			}
			attemptsLeft--;
		}
		if(!attemptsLeft){
			return -1;
		}
	}
	return hashVal;
}

/*
 *
 * name: insertHash
 *
 * Will locate a free position for the (word, code) in the given hashTable,
 * otherwise will return 0 if already in the table and -1 if the table is full.
 *
 * @param	hashTable	the built hash table to look up
 * @param	word	the word to hash and search for
 * @param	code	the type code to associate with the word
 * @return	1 upon successful insert, 0 on failure, and -1 on full 
 */
int insertHash(token ** hashTable, char * word, int code){
	int attemptsLeft = HASH_TABLE_SIZE;
	int hashVal = hash(word);
	token * newToken = malloc(sizeof(token));
	newToken->code = code;
	strncpy(newToken->name, word, MAX_TOKEN_LEN);
	if(hashTable[hashVal] == NULL){
		hashTable[hashVal] = newToken;
		return 1;
	}
	else{
		while(strcmp(hashTable[hashVal]->name, word) != 0 && attemptsLeft){
			hashVal = (hashVal+1) % HASH_TABLE_SIZE;
			if(hashTable[hashVal] == NULL){
				hashTable[hashVal] = newToken;
				return 1;
			}
			attemptsLeft--;
		}
		if(!attemptsLeft){
			// table full
			return -1;
		}
	}
	return 0;
}

/*
 *
 * name: buildHashes
 *
 * Generates the hash table from the given tokens using the hash() routine.
 * If a collision is detected, then linear probing is used.
 *
 * @param	hashTable	pointer to the hash table to be filled
 * @param	tokens		pointer to the token table to hash from
 */
void buildHashes(token ** hashTable, token * tokens){
	int i;
	for(i=0;i<MAX_TOKENS;i++){
		insertHash(hashTable, tokens[i].name, tokens[i].code);
	}
}

/*
 *
 * name: readTokens
 *
 * Opens the input parameter and reads contents into the token table.
 *
 * @param	tokens	the token table to be read into
 * @param	input	the file name string
 */
void readTokens(token * tokens, char * input){
	FILE* infile;
	int i;

	infile = fopen(input, "r");
	if(infile != NULL){
		for(i=0;i<MAX_TOKENS;i++){
			fscanf(infile, "%s %d", tokens[i].name,	&tokens[i].code);
		}
		fclose(infile);
	}
	else{
		printf("\n\nCould not open token file!\n");
		exit(1);
	}
}
