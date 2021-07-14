#include "ResourceAllocation.h"
#include "InterferenceGraph.h"
#include "Constants.h"
#include "Utils.h"
#include <set>
#include <vector>

using namespace std;


bool doResourceAllocation(SimplificationStack& simplificationStack, InterferenceGraph& ig)
{
	while (!simplificationStack.empty())
	{
		Variable* var = simplificationStack.top();
		simplificationStack.pop();

		std::set<Regs> colored;

		for (Variable* var2 : *(ig.variables))
		{
			if (ig.matrix[var->getPosition()][var2->getPosition()] == __INTERFERENCE__)
			{
				if (var2->getReg() != Regs::no_assign)
				{
					colored.insert(var2->getReg());
				}
			}
		}

		bool success = false;
		for (int r = 0; r < __REG_NUMBER__; ++r)
		{
			Regs reg = static_cast<Regs>(r);
			if (colored.find(reg) == colored.end())
			{
				var->setReg(reg);
				success = true;
				break;
			}
		}

		if (!success) 
			return false;

	}
	return true;
}


void changeRegsNameWithNewAllocation(Program& program)
{
	Instructions& instructions = program.getInstructions();
	for (Instruction* inst : instructions)
	{
		for (Variable* v : inst->getSrc())
		{
			if (v->getType() == Variable::REG_VAR)
				v->setName(getNameForRegister(v->getReg()));
		}
		for (Variable* v : inst->getDst())
		{
			if (v->getType() == Variable::REG_VAR)
				v->setName(getNameForRegister(v->getReg()));
		}
	}
}
