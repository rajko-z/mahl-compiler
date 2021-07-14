#pragma once

#include "Types.h"
#include "InterferenceGraph.h"
#include <stack>


bool doResourceAllocation(SimplificationStack& simplificationStack, InterferenceGraph& ig);

void changeRegsNameWithNewAllocation(Program& program);