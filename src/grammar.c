/*
 *      grammar.c
 *
 * These functions are the main BNF rules for the recursive descent parser.
 * There are extra utility functions also packaged here.
 *
 */


#include <stdio.h>

#include "config.h"
#include "tokens.h"
#include "hasher.h"
#include "scanner.h"
#include "grammar.h"

/*
 * name: err
 *
 * Prints a simple message.  Will attempt to print any scanner message within
 * the token first.
 *
 * @param	str	the string to print
 * @param	token	the current token which has caused the error.
 */
void err(char * str, superToken token){
	if(token.message[0] == '\0'){
		printf("\n----------\n(!) FAIL: %s\n(!) CURRENT TOKEN: %s\n----------\n",
				str, token.item.name);
	}
	else{
		printf("\n----------\n(!) FAIL: %s\n(!) CURRENT TOKEN: %s\n----------\n",
				token.message, token.item.name);
	}
}

/*
 * name: addId
 *
 * Will add the current token to the symbol table within the source structure
 * and set the type of the item to the given type.
 *
 * @param	source	the structure containing all parser information
 * @param	type	the type to set the item to
 * @return	0 upon error, 1 if successful	
 */
int addId(sourceContainer* source, int type){
	if(source->currentToken.error > 0){
		err(source->currentToken.message, source->currentToken);
		return 0;
	}
	int res = insertHash(source->symbolTable, source->currentToken.item.name, type);
	if(res == 0){
		err("Identifier already in symbol table", source->currentToken);
		return 0;
	}
	else if(res == -1){
		err("Symbol table full!", source->currentToken);
		return 0;
	}
	return 1;
}

/*
 * name: lookupId
 *
 * Will look up the current token in the symbol table within the source
 * structure.
 *
 * @param	source	the structure containing all parser information
 * @return	0 upon error, 1 if successful	
 */
int lookupId(sourceContainer* source){
	if(source->currentToken.error > 0){
		err(source->currentToken.message, source->currentToken);
		return 0;
	}
	if(-1 == getHash(source->symbolTable, source->currentToken.item.name)){
		err("Identifier not declared", source->currentToken);
		return 0;
	}
	return 1;
}

/*
 * name: prog
 *
 * The beginning function of our recursive descent.
 *
 * Rule: <prog> ::= PROGRAM <prog-name> VAR <dec-list> BEGIN <stmt-list> END.
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int prog(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	if (source->currentToken.item.code == PROGRAM){
		if (progName(source)){
			source->currentToken = getToken(source->current, source->infile, source->hashTable);
			if (source->currentToken.item.code == VAR){
				if (decList(source)){
					if (source->currentToken.item.code == BEGIN){
						if(stmtList(source)){
							if(source->currentToken.item.code == ENDDOT){
								return 1;
							}
							else {
								err("Expected END.", source->currentToken);
								return 0;
							}
						}
					}
					else {
						err("Expected BEGIN", source->currentToken);
						return 0;
					}
				}
			}
			else {
				err("Expected VAR", source->currentToken);
				return 0;
			}
		}
	}
	else {
		err("Expected PROGRAM", source->currentToken);
		return 0;
	}
	return 0;
}

/*
 * name: progName
 *
 * Rule: <prog-name> ::= id
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int progName(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	if(source->currentToken.item.code == ID){
		// add to symbol table
		if(!addId(source, 0)){
			return 0;
		}
		return 1;
	}
	else{
		err("Expected variable name", source->currentToken);
		return 0;
	}
	return 0;
}

/*
 * name: decList
 *
 * Rule: <dec-list> ::= <dec> | { ; <dec> }
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int decList(sourceContainer* source){
	if(dec(source)){
		source->currentToken = getToken(source->current, source->infile, source->hashTable);
		while(source->currentToken.item.code == SEMICOLON){
			if(!dec(source)){
				return 0;
			}
			source->currentToken = getToken(source->current, source->infile, source->hashTable);
		}
		return 1;
	}
	return 0;
}

/*
 * name: dec
 *
 * Rule: <dec> ::= <id-list> : <type>
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int dec(sourceContainer* source){
	if(idList(source, 1)){
		if(source->currentToken.item.code == COLON){
			if(type(source)){
				return 1;
			}
		}
		else{
			err("Expected :", source->currentToken);
			return 0;
		}
	}
	return 0;
}

/*
 * name: type
 *
 * Rule: <type> ::= INTEGER
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int type(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	if(source->currentToken.item.code == INTEGER){
		return 1;
	}
	else{
		err("Expected INTEGER", source->currentToken);
		return 0;
	}
	return 0;
}

/*
 * name: idList
 *
 * Rule: <id-list> ::= id | { , id }
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int idList(sourceContainer* source, int buildMode){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	if(source->currentToken.item.code == ID){
		if(buildMode){
			//add to symbol table
			if(!addId(source, 1)){
				return 0;
			}
		}
		else{
			//look up in symbol table
			if(!lookupId(source)){
				return 0;
			}
		}
		source->currentToken = getToken(source->current, source->infile, source->hashTable);
		while(source->currentToken.item.code == COMMA){
			source->currentToken = getToken(source->current, source->infile, source->hashTable);
			if(source->currentToken.item.code != ID){
				return 1;
			}
			if(buildMode && source->currentToken.item.code == ID){
				//add to symbol table
				if(!addId(source, 1)){
					return 0;
				}
			}
			else if(source->currentToken.item.code == ID){
				//look up in symbol table
				if(!lookupId(source)){
					return 0;
				}
			}
			source->currentToken = getToken(source->current, source->infile, source->hashTable);
		}
		return 1;
	}
	else{
		err("Expected variable name", source->currentToken);
		return 0;
	}
	return 0;
}


/*
 * name: stmtList
 *
 * Rule: <stmt-list> ::= <stmt> | { ; <stmt> }
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int stmtList(sourceContainer* source){
	if(stmt(source)){
		while(source->currentToken.item.code == SEMICOLON){
			if(!stmt(source)){
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

/*
 * name: stmt
 *
 * Rule: <stmt> ::= <assign> | <read> | <write> | <for>
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int stmt(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	switch(source->currentToken.item.code){
		case ID:
			//look up in symbol table
			if(!lookupId(source)){
				return 0;
			}
			return assign(source);
		case READ:
			if(read(source)){
				source->currentToken = getToken(source->current, source->infile, source->hashTable);
				return 1;
			}
			else{
				return 0;
			}
		case WRITE:
			if(write(source)){
				source->currentToken = getToken(source->current, source->infile, source->hashTable);
				return 1;
			}
			else{
				return 0;
			}
		case FOR:
			if(forStmt(source)){
				source->currentToken = getToken(source->current, source->infile, source->hashTable);
				return 1;
			}
			else{
				return 0;
			}
		case BEGIN:
			return 0;
		default:
			err("Expected statement", source->currentToken);
			return 0;
	}
}

/*
 * name: assign
 *
 * Rule: <assign> ::= id := <exp>
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int assign(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	if(source->currentToken.item.code == COLONEQUALS){
		if(expression(source)){
			return 1;
		}
	}
	else{
		err("Expected :=", source->currentToken);
		return 0;
	}
	return 0;
}

/*
 * name: expression
 *
 * Rule: <exp> ::= <term> { +- <term> }
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int expression(sourceContainer* source){
	if(term(source)){
		while(source->currentToken.item.code == PLUS || source->currentToken.item.code == MINUS){
			if(!term(source)){
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

/*
 * name: term
 *
 * Rule: <term> ::= <factor> { / DIV <factor> }
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int term(sourceContainer* source){
	if(factor(source)){
		source->currentToken = getToken(source->current, source->infile, source->hashTable);
		while(source->currentToken.item.code == ASTRIX || source->currentToken.item.code == DIV){
			if(!factor(source)){
				return 0;
			}
			source->currentToken = getToken(source->current, source->infile, source->hashTable);
		}
		return 1;
	}
	return 0;
}

/*
 * name: factor
 *
 * Rule: <factor> ::= id | int | ( <exp> )
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int factor(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	switch(source->currentToken.item.code){
		case PLUS:
		case MINUS:
			source->currentToken = getToken(source->current, source->infile, source->hashTable);
			if(source->currentToken.item.code == ID){
				if(!lookupId(source)){
					return 0;
				}
				else if(source->currentToken.error > 0){
					err("Invalid identifier format", source->currentToken);
					return 0;
				}
				return 1;
			}
			if(source->currentToken.item.code == INT){
				if(source->currentToken.error > 0){
					err("Invalid integer literal", source->currentToken);
					return 0;
				}
				return 1;
			}

			err("Expected identifier or literal", source->currentToken);
			return 0;
		case ID:
			// look up in symbol table
			if(!lookupId(source)){
				return 0;
			}
			return 1;
		case INT:
			if(source->currentToken.error > 0){
				err("Invalid literal", source->currentToken);
				return 0;
			}
			return 1;
		case LEFTPAREN:
			if(expression(source)){
				source->currentToken = getToken(source->current, source->infile, source->hashTable);
				if(source->currentToken.item.code == RIGHTPAREN){
					return 1;
				}
			}
		default:
			err("Expected identifier, literal, or expression.", source->currentToken);
			return 0;
	}
}

/*
 * name: read
 *
 * Rule: <read> ::= READ ( <id-list> )
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int read(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	if(source->currentToken.item.code == LEFTPAREN){
		if(idList(source, 0)){
			if(source->currentToken.item.code == RIGHTPAREN){
				return 1;
			}
			else{
				err("Expected )", source->currentToken);
				return 0;
			}
		}
	}
	else{
		err("Expected (", source->currentToken);
		return 0;
	}
	return 0;
}

/*
 * name: write
 *
 * Rule: <write> ::= WRITE ( <id-list> )
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int write(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	if(source->currentToken.item.code == LEFTPAREN){
		if(idList(source, 0)){
			if(source->currentToken.item.code == RIGHTPAREN){
				return 1;
			}
			else{
				err("Expected )", source->currentToken);
				return 0;
			}
		}
	}
	else{
		err("Expected (", source->currentToken);
		return 0;
	}
	return 0;
}

/*
 * name: forStmt
 *
 * Rule: <for> ::= FOR <index-exp> DO <body>
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int forStmt(sourceContainer* source){
	if(indexExp(source)){
		if(source->currentToken.item.code == DO){
			if(body(source)){
				return 1;
			}
		}
		else{
			err("Expected DO", source->currentToken);
			return 0;
		}
	}
	return 0;
}
	

/*
 * name: indexExp
 * 
 * Rule: <index-exp> ::= id := <exp> TO <exp>
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int indexExp(sourceContainer* source){
	source->currentToken = getToken(source->current, source->infile, source->hashTable);
	if(source->currentToken.item.code == ID){
		// look up in symbol table
		if(!lookupId(source)){
			return 0;
		}
		source->currentToken = getToken(source->current, source->infile, source->hashTable);
		if(source->currentToken.item.code == COLONEQUALS){
			if(expression(source)){
				if(source->currentToken.item.code == TO){
					if(expression(source)){
						return 1;
					}
				}
				else{
					err("Expected TO", source->currentToken);
					return 0;
				}
			}
		}
		else{
			err("Expected :=", source->currentToken);
			return 0;
		}
	}
	return 0;
}
	
/*
 * name: body
 *
 * Rule: <body> ::= <stmt> | BEGIN <stmt-list> END
 *
 * @param	source	structure containing all parser information
 * @return	0 upon failure, 1 if successful
 */
int body(sourceContainer* source){
	if(stmt(source)){
		return 1;
	}
	else{
		if(source->currentToken.item.code == BEGIN){
			if(stmtList(source)){
				if(source->currentToken.item.code == END){
					return 1;
				}
				else{
					err("Expected END", source->currentToken);
					return 0;
				}
			}
		}
		else{
			err("Expected BEGIN", source->currentToken);
			return 0;
		}
	}
	return 0;
}
