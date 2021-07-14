#pragma once

#include "IR.h"
#include "Program.h"


InterferenceGraph& buildInterferenceGraph(Program& program);

void printInterferenceMatrix();
