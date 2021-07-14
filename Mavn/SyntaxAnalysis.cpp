#include <iostream>
#include <iomanip>

#include "SyntaxAnalysis.h"

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin())
{
}

bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	
	Q();

	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}



void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			cout << currentToken.getValue() << endl;
			currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}

Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}



void SyntaxAnalysis::Q()
{
	if (currentToken.getType() == T_COMMENT)
	{
		eat(T_COMMENT);
		if (currentToken.getType() != T_END_OF_FILE)
		{
			Q();
		}

	}
	else
	{
		S();
		eat(T_SEMI_COL);
		L();
	}
}


void SyntaxAnalysis::S()
{
	if (currentToken.getType() == T_MEM)
	{
		eat(T_MEM);
		eat(T_M_ID);
		eat(T_NUM);
	}
	else if (currentToken.getType() == T_REG)
	{
		eat(T_REG);
		eat(T_R_ID);
	}
	else if (currentToken.getType() == T_FUNC)
	{
		eat(T_FUNC);
		eat(T_ID);
	}
	else if (currentToken.getType() == T_ID)
	{
		eat(T_ID);
		eat(T_COL);
		E();
	}
	else
	{
		E();
	}
}


void SyntaxAnalysis::L()
{
	if (currentToken.getType() != T_END_OF_FILE)
	{
		Q();
	}
}


void SyntaxAnalysis::E()
{
	if (currentToken.getType() == T_ADD)
	{
		eat(T_ADD);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);	
	}
	else if (currentToken.getType() == T_ADDI)
	{
		eat(T_ADDI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
	}
	else if (currentToken.getType() == T_SUB)
	{
		eat(T_SUB);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}
	else if (currentToken.getType() == T_LA)
	{
		eat(T_LA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_M_ID);
	}
	else if (currentToken.getType() == T_LW)
	{
		eat(T_LW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
	}
	else if (currentToken.getType() == T_LI)
	{
		eat(T_LI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
	}
	else if (currentToken.getType() == T_SW)
	{
		eat(T_SW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
	}
	else if (currentToken.getType() == T_B)
	{
		eat(T_B);
		eat(T_ID);
	}
	else if (currentToken.getType() == T_BLTZ)
	{
		eat(T_BLTZ);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
	}
	else if (currentToken.getType() == T_NOP)
	{
		eat(T_NOP);
	}
	else if (currentToken.getType() == T_AND)
	{
		eat(T_AND);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}
	else if (currentToken.getType() == T_SGE)
	{
		eat(T_SGE);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
	}
	else if (currentToken.getType() == T_BEQ)
	{
		eat(T_BEQ);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
	}
}
