#pragma once
#include <string>

class FileParse
{
public:

	FileParse(const std::string& filepath);

	void ParseFile(const std::string& filepath);

private:
	std::string m_FilePath;
};
