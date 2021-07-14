#include "SimplificationStack.h"
#include <algorithm>
#include <map>
#include <set>

using namespace std;

static SimplificationStack sp;

int getNextVariablePosForStack(InterferenceMatrix& matrix, set<int>& already_used, const int degree)
{
	int maxx = -1;
	int maxPos = -1;
	for (int i = 0; i < matrix.size(); ++i)
	{	
		if (already_used.find(i) != already_used.end())
			continue;

		int tmp = 0;
		for (int j = 0; j < matrix.size(); ++j)
		{
			if (matrix[i][j] == __INTERFERENCE__)
			{
				++tmp;
			}
		}
		if (tmp <= (degree - 1))
		{
			if (tmp > maxx)
			{
				maxx = tmp;
				maxPos = i;
			}
		}
	}
	if (maxx != -1)
	{
		return maxPos;
	}
	throw runtime_error("Split detected while building SimplificationStack.");
}



void removeLinksWithVariable(int pos, InterferenceMatrix& matrix)
{
	for (int i = 0; i < matrix.size(); ++i)
	{
		matrix[pos][i] = __EMPTY__;
		matrix[i][pos] = __EMPTY__;
	}
}

SimplificationStack& doSimplification(InterferenceGraph& ig, const int degree)
{
	InterferenceMatrix matrix = ig.matrix; // make copy
	map<int, Variable*> var_map; // for faster lookup

	for (Variable* v : (*ig.variables))
	{
		var_map[v->getPosition()] = v;
	}

	int brVars = 0;
	int varsSize = ig.variables->size();
	set<int> already_used;
	while (brVars < varsSize)
	{	
		int varPos = getNextVariablePosForStack(matrix, already_used, degree);
		sp.push(var_map[varPos]);
		removeLinksWithVariable(varPos, matrix);
		++brVars;
		already_used.insert(varPos);
	}

	return sp;
}

void printSimplificationStack()
{
	if (sp.size() == 0)
	{
		cout << "There is nothing to print!" << endl;
		return;
	}

	cout << "==========================================" << endl;
	cout << "Simplification stack (from top to bottom):" << endl;
	cout << "==========================================" << endl;


	SimplificationStack copy = sp;
	while (!copy.empty())
	{
		Variable* top = copy.top();
		copy.pop();
		cout << top->getName() << " ";
	}
	cout << "\n";
}