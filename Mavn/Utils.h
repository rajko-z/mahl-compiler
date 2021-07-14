#pragma once
#include <list>
#include <string>
#include "IR.h"

bool compareVarPt(Variable* var1, Variable* var2);

bool compareVar(Variable var1, Variable var2);

bool compareUniqueness(Variable* var1, Variable* var2);

bool listsAreDifferent(std::list<Variable*>& list1, std::list<Variable>& list2);

bool variableExists(Variable* var, Variables& vars);

std::string getNameForRegister(Regs reg);