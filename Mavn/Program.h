#pragma once
#include "LexicalAnalysis.h"
#include "IR.h"
#include <map>
#include <list>
#include <vector>
#include <set>

class Program {

public:
	Program(LexicalAnalysis& lex);

	void createFlowGraph();
	void printInstructions();
	void printSextionData();
	void printFlowGraph();

	Instructions& getInstructions() { return m_instructions; }
	const Instructions& getInstructions() const { return m_instructions; }
	const std::set<std::string>& getFunctions() const { return m_functions; }
	const std::map<std::string, Variable*>getMemoryMap() const { return m_mem_map; }
	const std::map<std::string, int>& getLabelMap() const { return m_label_map; }
	//std::map<std::string, int> getLabelMap() const

private:
	int m_instruction_counter = 0;

	LexicalAnalysis& m_lex;
	TokenList::iterator m_tokenIterator;
	Token m_currentToken;

	std::map<std::string, Variable*> m_mem_map;
	std::map<std::string, Variable*> m_reg_map;
	std::set<std::string> m_functions;
	std::map<std::string, int> m_label_map;
	Instructions m_instructions;
	std::map<int, Instruction*> m_inst_map;

private:

	Token getNextToken();

	void createSectionsAndInstructions();
	void addLinksBetweenInstructions();

	void checkIfRegIsUsedBeforeDeclaring(std::string regName);
	void checkIfMemIsUsedBeforeDeclaring(std::string memName);
	void checkIfLabelDoesNotExist(std::string labelName);
	void removePredAndSuccInstrDuplicates();


	// -----------------------
	void createMemVariable();
	void createRegVariable();
	void createFuncVariable();
	void createLabel();
	void createAdd_inst();
	void createAddi_inst();
	void createSub_inst();
	void createLa_inst();
	void createLi_inst();
	void createLw_inst();
	void createSw_inst();
	void createBltz_inst();
	void createB_inst();
	void createNop_inst();
	void createAnd_inst();
	void createSge_inst();
	void createBeq_inst();
};