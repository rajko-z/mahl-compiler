#ifndef __IR__
#define __IR__

#include <string>
#include "Types.h"


/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() :
		m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}

	Variable(std::string name, VariableType type) 
		: m_type(type), m_name(name), m_position(-1), m_assignment(no_assign) {}

	Variable(std::string name, std::string value, VariableType type) :
		m_value(value), m_type(type), m_name(name), m_position(-1), m_assignment(no_assign) {}

	Variable(std::string name, int pos) :
		m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}

	Variable(const Variable& var) :
		m_name(var.getName()) {}

	std::string getName() const { return m_name; }
	void setName(const std::string& name)  { this->m_name = name; }


	std::string getValue() const { return m_value; }
	int getPosition() const { return m_position;  }
	void setPosition(int pos) { this->m_position = pos; }
	Regs getReg() const { return m_assignment; }
	void setReg(Regs reg) { this->m_assignment = reg; }
	VariableType getType() const { return this->m_type; }

private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	std::string m_value;
	Regs m_assignment;
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () :
		m_position(0), m_type(I_NO_TYPE) {}

	Instruction(std::string asmString, int pos, InstructionType type) :
		m_position(pos), m_type(type), m_asmString(asmString) {}

	Instruction (std::string asmString, int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), m_asmString(asmString) {}
	
	Instruction(std::string asmString, int pos, InstructionType type, Variables& dst,
				Variables& src, Variables& def, Variables& use) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), m_asmString(asmString), m_use(use), m_def(def) {}

	Instruction(std::string asmString, int pos, InstructionType type, Variables& dst,
				Variables& src, Variables& def, Variables& use, std::string labelName) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), m_asmString(asmString), m_use(use), m_def(def), m_labelName(labelName) {}


	/* Gets full instruction string. */
	std::string toString() const;

	std::string getAsmString() const { return m_asmString; }

	std::string getLabelName() const { return m_labelName;  }

	InstructionType getType() const { return m_type; }

	int getPos() const { return m_position; }

	Variables& getDst() { return m_dst; }
	const Variables& getDst() const { return m_dst; }

	Variables& getSrc() { return m_src; }
	const Variables& getSrc() const { return m_src; }

	Variables& getUse() { return m_use; }
	const Variables& getUse() const { return m_use; }

	Variables& getDef() { return m_def; }
	const Variables& getDef() const { return m_def; }

	Variables& getIn() { return m_in; }
	const Variables& getIn() const { return m_in; }

	Variables& getOut() { return m_out; }
	const Variables& getOut() const { return m_out; }

	std::list<Instruction*>& getPred() { return m_pred; }
	const std::list<Instruction*>& getPred() const { return m_pred; }

	std::list<Instruction*>& getSucc() { return m_succ; }
	const std::list<Instruction*>& getSucc() const { return m_succ; }


private:
	int m_position;
	std::string m_labelName; // only for jump instructions

	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;

	std::string m_asmString;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;


/**
 * This type represents matrix of integers.
 */
typedef std::vector<std::vector<int>> InterferenceMatrix;


/**
 * This type represents stack of pointers to variables.
 */
typedef std::stack<Variable*> SimplificationStack;

/**
 * This type represents interference graph.
 */
struct InterferenceGraph
{
	Variables* variables;		// all variables from instructions
	InterferenceMatrix matrix;	// interference matrix
};







#endif
