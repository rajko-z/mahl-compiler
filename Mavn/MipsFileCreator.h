#pragma once
#include "Program.h"
#include <string>
#include <fstream>

class MipsFileCreator {
public:
	MipsFileCreator(Program& program, std::string fileName);
	void create();

private:
	void createGloblSection(std::ofstream& file);
	void createDataSection(std::ofstream& file);
	void createTextSection(std::ofstream& file);

	Program& m_program;
	std::string m_fileName;

};