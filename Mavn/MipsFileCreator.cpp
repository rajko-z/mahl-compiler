#include "MipsFileCreator.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

MipsFileCreator::MipsFileCreator(Program& program, string fileName)
	: m_program(program), m_fileName(fileName)
{
}

void MipsFileCreator::create()
{
	ofstream mipsFile{ m_fileName };

	if (!mipsFile)
	{
		ostringstream ss;
		ss << "Bad file name: " << m_fileName << '\n';
		throw runtime_error(ss.str());
	}

	createGloblSection(mipsFile);
	createDataSection(mipsFile);
	createTextSection(mipsFile);
}

void MipsFileCreator::createGloblSection(std::ofstream& file)
{
	const std::set<string>& functions = m_program.getFunctions();
	for (string s : functions)
	{
		file << ".globl " << s << '\n';
	}
	file << '\n';
}

void MipsFileCreator::createDataSection(std::ofstream& file)
{
	const std::map<string, Variable*>& memomoryMap = m_program.getMemoryMap();

	file << ".data\n";
	for (auto it : memomoryMap)
	{
		file << it.first << ':' << " .word " << it.second->getValue() << '\n';
	}
	file << '\n';
}

void MipsFileCreator::createTextSection(std::ofstream& file)
{
	const map<string, int>& labelMap = m_program.getLabelMap();
	map<int, string> orderedLabels;    // using map as BST ordered tree for positions of labels

	for (auto it : labelMap)
	{
		orderedLabels.insert({ it.second, it.first });
	}

	file << ".text\n";

	Instructions& instructions = m_program.getInstructions();
	int counter = 0;
	for (Instruction* inst : instructions)
	{
		if (orderedLabels.find(counter) != orderedLabels.end())
		{
			file << orderedLabels[counter] << ":\n";
		}
		file << "\t" << inst->toString() << '\n';
		++counter;
	}
}