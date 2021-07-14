#pragma once

#include "Token.h"
#include "LexicalAnalysis.h"

class SyntaxAnalysis
{
public:

	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis& lex);

	/**
	* Method which performs syntax analysis
	*/
	bool Do();

private:
	/**
	* Prints the error message, and token that caused the syntax error
	*/
	void printSyntaxError(Token token);


	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/**
	* Returns the next token from the token list
	*/
	Token getNextToken();

	/**
	 * Nonterminal Q
	 */
	void Q();

	/**
	 * Nonterminal S
	 */
	void S();

	/**
	 * Nonterminal L
	 */
	void L();

	/**
	 * Nonterminal E
	 */
	void E();


	/**
	* Reference to lexical analysis module
	*/
	LexicalAnalysis& lexicalAnalysis;

	/**
	* Syntax error indicator
	*/
	bool errorFound;

	/**
	* Iterator to the token list which represents the output of the lexical analysis
	*/
	TokenList::iterator tokenIterator;

	/**
	* Current token that is being analyzed
	*/
	Token currentToken;
};
