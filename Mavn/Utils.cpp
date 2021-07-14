#include "Utils.h"

using namespace std;

bool compareVarPt(Variable* var1, Variable* var2)
{
	return var1->getName() < var2->getName();
}
bool compareVar(Variable var1, Variable var2)
{
	return var1.getName() < var2.getName();
}
bool compareUniqueness(Variable* var1, Variable* var2)
{
	return var1->getName() == var2->getName();
}

bool listsAreDifferent(std::list<Variable*>& list1, std::list<Variable>& list2)
{
	if (list1.size() != list2.size())
		return true;

	list1.sort(compareVarPt);
	list2.sort(compareVar);

	auto it = list2.begin();
	for (Variable* var : list1)
	{
		if (var->getName() != it->getName())
		{
			return true;
		}
		++it;
	}
	return false;
}

bool variableExists(Variable* var, Variables& vars)
{
	for (Variable* v : vars)
	{
		if (v->getName() == var->getName())
			return true;
	}
	return false;
}

string getNameForRegister(Regs reg)
{
	switch (reg)
	{
	case no_assign:
		break;
	case t0:
		return "t0";
	case t1:
		return "t1";
	case t2:
		return "t2";
	case t3:
		return "t3";
	case t4:
		return "t4";
	default:
		break;
	}
}