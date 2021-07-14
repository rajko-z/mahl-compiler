#include "LivenessAnalysis.h"
#include <iostream>
#include "Utils.h"

using namespace std;

void livenessAnalysis(Program& program)
{	
	Instructions& instructions = program.getInstructions();

	while (true)
	{
		std::list<std::list<Variable>> inCopy;
		std::list<std::list<Variable>> outCopy;
	
		for (Instruction* instr : instructions)
		{
			std::list<Variable> inToAdd;
			for (Variable* v : instr->getIn())
			{
				inToAdd.push_back(Variable(*v));
			}
			inCopy.push_back(inToAdd);

			std::list<Variable> outToAdd;
			for (Variable* v : instr->getOut())
			{
				outToAdd.push_back(Variable(*v));
			}
			outCopy.push_back(outToAdd);
		}


		for (Instructions::reverse_iterator it = instructions.rbegin(); it != instructions.rend(); ++it)
		{
			Instruction* instr = *it;
			for (Instruction* s : instr->getSucc())
			{
				instr->getOut().insert(instr->getOut().end(), s->getIn().begin(), s->getIn().end());
			}

			instr->getIn().insert(instr->getIn().end(), instr->getUse().begin(), instr->getUse().end());

			for (Variable* v : instr->getOut())
			{
				if (!variableExists(v, instr->getDef()))
				{
					instr->getIn().push_back(v);
				}
			}
			instr->getOut().sort(compareVarPt); 
			instr->getOut().unique(compareUniqueness);
			instr->getIn().sort(compareVarPt);
			instr->getIn().unique(compareUniqueness);
		}



		bool stillDifferent = false;
		auto itIn = inCopy.begin();
		auto itOut = outCopy.begin();

		for (Instruction* instr : instructions)
		{
			if (listsAreDifferent(instr->getIn(), *itIn)) 
			{
				stillDifferent = true;
				break;
			}
			if (listsAreDifferent(instr->getOut(), *itOut)) 
			{
				stillDifferent = true;
				break;
			}
			++itIn;
			++itOut;
		}

		if (!stillDifferent) 
			break;

	}


}
