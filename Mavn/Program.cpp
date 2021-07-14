#include "Program.h"
#include <iostream>
#include <sstream>

using namespace std;

Program::Program(LexicalAnalysis& lex)
	:m_lex(lex), m_tokenIterator(m_lex.getTokenList().begin())
{
}

void Program::createFlowGraph()
{
	createSectionsAndInstructions();
	addLinksBetweenInstructions();
}


void Program::addLinksBetweenInstructions()
{
	Instructions::iterator itCurr = m_instructions.begin();
	Instructions::iterator itPrev = m_instructions.begin();

	itCurr++;

	while (itCurr!= m_instructions.end())
	{
		(*itCurr)->getPred().push_back(*itPrev);
		(*itPrev)->getSucc().push_back(*itCurr);

		if ((*itCurr)->getType() == I_B || (*itCurr)->getType() == I_BEQ || (*itCurr)->getType() == I_BLTZ)
		{
			string goToLabel = (*itCurr)->getLabelName();
			checkIfLabelDoesNotExist(goToLabel);
			int instPos = m_label_map[goToLabel];

			(*itCurr)->getSucc().push_back(m_inst_map[instPos]);
			m_inst_map[instPos]->getPred().push_back(*itCurr);
		}
		itCurr++;
		itPrev++;
	}
	removePredAndSuccInstrDuplicates();

}

void Program::removePredAndSuccInstrDuplicates()
{
	for (Instruction* inst : m_instructions)
	{
		inst->getPred().unique([](Instruction* one, Instruction* two) { return one->getPos() == two->getPos(); });
		inst->getSucc().unique([](Instruction* one, Instruction* two) { return one->getPos() == two->getPos(); });
	}
}

void Program::checkIfLabelDoesNotExist(std::string labelName)
{
	if (m_label_map.find(labelName) == m_label_map.end() &&
		m_functions.find(labelName) == m_functions.end())
	{
		ostringstream ss;
		ss << "Error. Can't branch. There is no label with name " << labelName << "\n";
		throw runtime_error(ss.str());
	}
}

void Program::checkIfRegIsUsedBeforeDeclaring(string regName)
{
	if (m_reg_map.find(regName) == m_reg_map.end())
	{
		ostringstream ss;
		ss << "Error. Reg with name " << regName << " is used but never declared\n";
		throw runtime_error(ss.str());
	}
}

void Program::checkIfMemIsUsedBeforeDeclaring(string memName)
{
	if (m_mem_map.find(memName) == m_mem_map.end())
	{
		ostringstream ss;
		ss << "Error. Mem variable with name " << memName << " is used but never declared\n";
		throw runtime_error(ss.str());
	}
}

Token Program::getNextToken()
{
	if (m_tokenIterator == m_lex.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *m_tokenIterator++;
}

void Program::printInstructions()
{
	for (auto it : m_inst_map)
	{
		cout << it.first << " " << it.second->toString() << '\n';
	}
}

void Program::printSextionData()
{
	cout << "===============MEMORY MAP====================\n";
	for (auto it : m_mem_map)
	{
		cout << it.first << " " << it.second->getValue() << '\n';
	}
	cout << '\n';

	cout << "===============REG MAP====================\n";
	for (auto it : m_reg_map)
	{
		cout << it.first << '\n';
	}
	cout << '\n';


	cout << "==================FUNCTIONS====================\n";
	for (auto it : m_functions)
	{
		cout << it << '\n';
	}
	cout << '\n';


	cout << "========================LABEL====================\n";
	for (auto it : m_label_map)
	{
		cout << it.first << " " << it.second << "\n";
	}
	cout << '\n';
}

void Program::printFlowGraph()
{
	for (Instruction* inst : m_instructions)
	{
		cout << "------------------------------------------\n";
		cout << inst->getPos() << " " << inst->toString() << '\n';
		cout << "PRED:\n";
		for (Instruction* i : inst->getPred())
		{
			cout << "\t" << i->getPos() << " " << i->toString() << '\n';
		}

		cout << "SUCC:\n";
		for (Instruction* i : inst->getSucc())
		{
			cout << "\t" << i->getPos() << " " << i->toString() << '\n';
		}

		cout << "USE:\n\t";
		for (Variable* v : inst->getUse())
		{
			cout << v->getName() << " ";
		}

		cout << "\nDEF:\n\t";
		for (Variable* v : inst->getDef())
		{
			cout << v->getName() << " ";
		}

		cout << "\nIN:\n\t";
		for (Variable* v : inst->getIn())
		{
			cout << v->getName() << " ";
		}

		cout << "\nOUT:\n\t";
		for (Variable* v : inst->getOut())
		{
			cout << v->getName() << " ";
		}
		cout << '\n';
		cout << "------------------------------------------\n";
	}
}




//                 CREATE STRUCTURE OF PROGRAM
// ==========================================================================================

void Program::createSectionsAndInstructions() 
{
	while (m_tokenIterator != m_lex.getTokenList().end())
	{
		m_currentToken = getNextToken();

		switch (m_currentToken.getType())
		{
		case T_MEM:  { createMemVariable();  break; }
		case T_REG:  { createRegVariable();  break; }
		case T_FUNC: { createFuncVariable(); break; }
		case T_ID:   { createLabel();        break; }
		case T_ADD:  { createAdd_inst();     break; }
		case T_ADDI: { createAddi_inst();    break; }
		case T_SUB:  { createSub_inst();     break; }
		case T_LA:   { createLa_inst();      break; }
		case T_LI:   { createLi_inst();      break; }
		case T_LW:   { createLw_inst();      break; }
		case T_SW:   { createSw_inst();      break; }
		case T_BLTZ: { createBltz_inst();    break; }
		case T_B:    { createB_inst();       break; }
		case T_NOP:  { createNop_inst();     break; }
		case T_AND:  { createAnd_inst();     break; }
		case T_SGE:  { createSge_inst();     break; }
		case T_BEQ:  { createBeq_inst();     break; }
		default:
			break;
		}
	}
}

void Program::createMemVariable()
{
	Token mem_var = getNextToken();
	Token mem_value = getNextToken();
	Token semi = getNextToken();

	if (m_mem_map.find(mem_var.getValue()) != m_mem_map.end())
	{
		ostringstream ss;
		ss << "Error. Memory variable " << mem_var.getValue() << " already declared.\n";
		throw runtime_error(ss.str());
	}
	else
	{
		Variable* v = new Variable(mem_var.getValue(), mem_value.getValue(), Variable::MEM_VAR);
		m_mem_map[mem_var.getValue()] = v;
	}
}

void Program::createRegVariable()
{
	Token reg_var = getNextToken();
	Token semi = getNextToken();

	if (m_reg_map.find(reg_var.getValue()) != m_reg_map.end())
	{
		ostringstream ss;
		ss << "Error. Registar variable " << reg_var.getValue() << " already declared.\n";
		throw runtime_error(ss.str());
	}
	else
	{
		Variable* v = new Variable(reg_var.getValue(), Variable::REG_VAR);
		m_reg_map[reg_var.getValue()] = v;
	}
}

void Program::createFuncVariable()
{
	Token func_name = getNextToken();
	Token semi = getNextToken();

	if (m_functions.find(func_name.getValue()) != m_functions.end())
	{
		ostringstream ss;
		ss << "Error. Function " << func_name.getValue() << " already declared.\n";
		throw runtime_error(ss.str());
	}
	else if (m_label_map.find(func_name.getValue()) != m_label_map.end())
	{
		ostringstream ss;
		ss << "Error. Can't declare function " << func_name.getValue() << " because there is already label with this name\n";
		throw runtime_error(ss.str());
	}
	else
	{
		m_functions.insert(func_name.getValue());
		m_label_map[func_name.getValue()] = m_instruction_counter;
	}
}

void Program::createLabel()
{
	Token colon = getNextToken();

	if (m_functions.find(m_currentToken.getValue()) != m_functions.end())
	{
		ostringstream ss;
		ss << "Error. Invalid lable with name " << m_currentToken.getValue() << " because there is already a function delcared with this name\n";
		throw runtime_error(ss.str());
	}
	else if (m_label_map.find(m_currentToken.getValue()) != m_label_map.end())
	{
		ostringstream ss;
		ss << "Error. Label with name " << m_currentToken.getValue() << " is already declared\n";
		throw runtime_error(ss.str());
	}
	else
	{
		m_label_map[m_currentToken.getValue()] = m_instruction_counter;
	}
}

void Program::createAdd_inst()
{
	Token reg_dest = getNextToken();
	Token comma = getNextToken();
	Token reg_src1 = getNextToken();
	comma = getNextToken();
	Token reg_src2 = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_dest.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src1.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src2.getValue());

	Variables dest;
	dest.push_back(m_reg_map[reg_dest.getValue()]);

	Variables def;
	def.push_back(m_reg_map[reg_dest.getValue()]);

	Variables src;
	src.push_back(m_reg_map[reg_src1.getValue()]);
	src.push_back(m_reg_map[reg_src2.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src1.getValue()]);
	use.push_back(m_reg_map[reg_src2.getValue()]);

	string asmString = "add $`d, $`s, $`s";
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_ADD, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createAddi_inst()
{
	Token reg_dest = getNextToken();
	Token comma = getNextToken();
	Token reg_src = getNextToken();
	comma = getNextToken();
	Token constant = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_dest.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src.getValue());

	Variables dest;
	dest.push_back(m_reg_map[reg_dest.getValue()]);

	Variables def;
	def.push_back(m_reg_map[reg_dest.getValue()]);

	Variables src;
	src.push_back(m_reg_map[reg_src.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src.getValue()]);

	string asmString = "addi $`d, $`s, " + constant.getValue();
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_ADDI, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createSub_inst()
{
	Token reg_dest = getNextToken();
	Token comma = getNextToken();
	Token reg_src1 = getNextToken();
	comma = getNextToken();
	Token reg_src2 = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_dest.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src1.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src2.getValue());

	Variables dest;
	dest.push_back(m_reg_map[reg_dest.getValue()]);

	Variables def;
	def.push_back(m_reg_map[reg_dest.getValue()]);

	Variables src;
	src.push_back(m_reg_map[reg_src1.getValue()]);
	src.push_back(m_reg_map[reg_src2.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src1.getValue()]);
	use.push_back(m_reg_map[reg_src2.getValue()]);

	string asmString = "sub $`d, $`s, $`s";
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_SUB, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createLa_inst()
{
	Token reg_dest = getNextToken();
	Token comma = getNextToken();
	Token mem_src = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_dest.getValue());
	checkIfMemIsUsedBeforeDeclaring(mem_src.getValue());

	Variables dest;
	dest.push_back(m_reg_map[reg_dest.getValue()]);

	Variables def;
	def.push_back(m_reg_map[reg_dest.getValue()]);

	Variables src;
	src.push_back(m_mem_map[mem_src.getValue()]);

	Variables use;

	string asmString = "la $`d, `s";
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_LA, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createLi_inst()
{
	Token reg_dest = getNextToken();
	Token comma = getNextToken();
	Token constant = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_dest.getValue());

	Variables dest;
	dest.push_back(m_reg_map[reg_dest.getValue()]);

	Variables def;
	def.push_back(m_reg_map[reg_dest.getValue()]);

	Variables src;
	Variables use;

	string asmString = "li $`d, " + constant.getValue();
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_LA, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createLw_inst()
{
	Token reg_dest = getNextToken();
	Token comma = getNextToken();
	Token constant = getNextToken();
	Token leftParent = getNextToken();
	Token reg_src = getNextToken();
	Token rightParent = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_dest.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src.getValue());

	Variables dest;
	dest.push_back(m_reg_map[reg_dest.getValue()]);

	Variables def;
	def.push_back(m_reg_map[reg_dest.getValue()]);

	Variables src;
	src.push_back(m_reg_map[reg_src.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src.getValue()]);

	string asmString = "lw $`d, " + constant.getValue() + "($`s)";
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_LW, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createSw_inst()
{
	Token reg_src1 = getNextToken();
	Token comma = getNextToken();
	Token constant = getNextToken();
	Token leftParent = getNextToken();
	Token reg_src2 = getNextToken();
	Token rightParent = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_src1.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src2.getValue());

	Variables dest;
	Variables def;

	Variables src;
	src.push_back(m_reg_map[reg_src1.getValue()]);
	src.push_back(m_reg_map[reg_src2.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src1.getValue()]);
	use.push_back(m_reg_map[reg_src2.getValue()]);

	string asmString = "sw $`s, " + constant.getValue() + "($`s)";
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_SW, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createBltz_inst()
{
	Token reg_src = getNextToken();
	Token comma = getNextToken();
	Token label = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_src.getValue());

	Variables src;
	src.push_back(m_reg_map[reg_src.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src.getValue()]);

	Variables dest;
	Variables def;

	string asmString = "bltz $`s, " + label.getValue();
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_BLTZ, dest, src, def, use, label.getValue());

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });

}

void Program::createB_inst()
{
	Token label = getNextToken();
	Token semi = getNextToken();

	Variables src;
	Variables dest;
	Variables use;
	Variables def;

	string asmString = "b " + label.getValue();
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_BLTZ, dest, src, def, use, label.getValue());

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createNop_inst()
{
	Token semi = getNextToken();
	string asmString = "nop";
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_NOP);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createAnd_inst()
{
	Token reg_dest = getNextToken();
	Token comma = getNextToken();
	Token reg_src1 = getNextToken();
	comma = getNextToken();
	Token reg_src2 = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_dest.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src1.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src2.getValue());

	Variables dest;
	dest.push_back(m_reg_map[reg_dest.getValue()]);

	Variables def;
	def.push_back(m_reg_map[reg_dest.getValue()]);

	Variables src;
	src.push_back(m_reg_map[reg_src1.getValue()]);
	src.push_back(m_reg_map[reg_src2.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src1.getValue()]);
	use.push_back(m_reg_map[reg_src2.getValue()]);

	string asmString = "and $`d, $`s, $`s";
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_AND, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createSge_inst()
{
	Token reg_dest = getNextToken();
	Token comma = getNextToken();
	Token reg_src1 = getNextToken();
	comma = getNextToken();
	Token reg_src2 = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_dest.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src1.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src2.getValue());

	Variables dest;
	dest.push_back(m_reg_map[reg_dest.getValue()]);

	Variables def;
	def.push_back(m_reg_map[reg_dest.getValue()]);

	Variables src;
	src.push_back(m_reg_map[reg_src1.getValue()]);
	src.push_back(m_reg_map[reg_src2.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src1.getValue()]);
	use.push_back(m_reg_map[reg_src2.getValue()]);

	string asmString = "sge $`d, $`s, $`s";
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_SGE, dest, src, def, use);

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

void Program::createBeq_inst()
{
	Token reg_src1 = getNextToken();
	Token comma = getNextToken();
	Token reg_src2 = getNextToken();
	comma = getNextToken();
	Token label = getNextToken();
	Token semi = getNextToken();

	checkIfRegIsUsedBeforeDeclaring(reg_src1.getValue());
	checkIfRegIsUsedBeforeDeclaring(reg_src2.getValue());

	Variables src;
	src.push_back(m_reg_map[reg_src1.getValue()]);
	src.push_back(m_reg_map[reg_src2.getValue()]);

	Variables use;
	use.push_back(m_reg_map[reg_src1.getValue()]);
	use.push_back(m_reg_map[reg_src2.getValue()]);

	Variables dest;
	Variables def;

	string asmString = "beq $`s, $`s, " + label.getValue();
	int pos = m_instruction_counter++;

	Instruction* inst = new Instruction(asmString, pos, I_BEQ, dest, src, def, use, label.getValue());

	m_instructions.push_back(inst);
	m_inst_map.insert({ pos, inst });
}

//============================================================================================