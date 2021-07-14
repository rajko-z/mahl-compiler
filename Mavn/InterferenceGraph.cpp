#include "InterferenceGraph.h"
#include "Utils.h"

#include <vector>
#include <list>

using namespace std;

static InterferenceGraph ig;

InterferenceGraph& buildInterferenceGraph(Program& program)
{	
	Instructions& instructions = program.getInstructions();
	list<Variable*>* vlist = new list<Variable*>();
	ig.variables = vlist;

	for (Instruction* ins : instructions)
	{
		for (Variable* v : ins->getDef())
		{
			ig.variables->push_back(v);
		}
		for (Variable* v : ins->getIn())
		{
			ig.variables->push_back(v);
		}
	}
	vlist->sort(compareVarPt);
	vlist->unique(compareUniqueness);

	// set positions
	int pos = 0;
	for (Variable* v : *vlist)
	{
		v->setPosition(pos++);
	}
	
	// initMatrix
	for (int i = 0; i < ig.variables->size(); ++i) {
		vector<int> row;
		for (int j = 0; j < ig.variables->size(); ++j) {
			row.push_back(__EMPTY__);
		}
		ig.matrix.push_back(row);
	}

	// add interferences
	for (Instruction* ins : instructions)
	{
		for (Variable* v : ins->getDef())
		{
			for (Variable* o : ins->getOut())
			{
				if (v->getName() != o->getName())
				{
					ig.matrix[v->getPosition()][o->getPosition()] = __INTERFERENCE__;
					ig.matrix[o->getPosition()][v->getPosition()] = __INTERFERENCE__;
				}
			}
		}
	}

	return ig;
}


void printInterferenceMatrix()
{
	if (ig.matrix.size() == 0)
	{
		cout << "There is nothing to print!" << endl;
		return;
	}

	cout << "==========================================" << endl;
	cout << "Interference matrix:" << endl;
	cout << "==========================================" << endl;

	// print existing variables in order to mark rows and columns
	for (auto varIt = ig.variables->begin(); varIt != ig.variables->end(); varIt++)
	{
		cout << "\t" << (*varIt)->getName();
	}
	cout << endl;

	auto varIt = ig.variables->begin();
	for (auto it1 = ig.matrix.begin(); it1 != ig.matrix.end(); it1++)
	{
		cout << (*varIt++)->getName();
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
		{
			cout << "\t" << *it2;
		}
		cout << endl;
	}
	cout << "==========================================" << endl;
}
