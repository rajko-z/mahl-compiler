#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "Program.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"
#include "SimplificationStack.h"
#include "ResourceAllocation.h"
#include "MipsFileCreator.h"

using namespace std;

int main()
{
	try
	{
		//std::string fileNameIn = ".\\..\\examples\\addFirstN.mavn";
		//std::string fileNameOut = ".\\..\\examples\\addFirstNMips.s";

		std::string fileNameIn = ".\\..\\examples\\multiply.mavn";
		std::string fileNameOut = ".\\..\\examples\\multiplyMips.s";

		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileNameIn))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}


		SyntaxAnalysis syntax(lex);
		retVal = syntax.Do();

		if (retVal)
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}

		Program program(lex);
		program.createFlowGraph();
		program.printFlowGraph();

		livenessAnalysis(program);

		cout << "==================================\n";
		cout << "       AFTER LIVENESS_ANALYSIS    \n";
		cout << "==================================\n";

		program.printFlowGraph();


		InterferenceGraph& ig = buildInterferenceGraph(program);
		printInterferenceMatrix();


		SimplificationStack& sp = doSimplification(ig, __REG_NUMBER__);
		printSimplificationStack();

		retVal = doResourceAllocation(sp, ig);
		if (retVal)
		{
			changeRegsNameWithNewAllocation(program);
			cout << "===================================\n";
			cout << "        FINAL INSTRUCTIONS         \n";
			cout << "===================================\n";
			program.printInstructions();

			MipsFileCreator mipsFile(program, fileNameOut);
			mipsFile.create();
		}
		else
		{
			throw runtime_error("\nSpill detected!\n");
		}

	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
