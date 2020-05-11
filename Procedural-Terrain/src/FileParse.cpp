#include "FileParse.h"
#include <sstream>
#include <iostream>
#include <fstream>

FileParse::FileParse(const std::string& filepath) : m_FilePath(filepath)
{
	ParseFile(m_FilePath);
}

void FileParse::ParseFile(const std::string& filepath)
{
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss;
	while (std::getline(stream, line))
	{
		ss << line << '\n';
	}
	std::cout << ss.str() << std::endl;
}
