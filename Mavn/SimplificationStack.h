#pragma once

#include "IR.h"

#include <stack>

SimplificationStack& doSimplification(InterferenceGraph& ig, const int degree);
void printSimplificationStack();
