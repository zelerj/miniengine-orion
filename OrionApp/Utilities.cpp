#include "stdafx.h"
#include "Utilities.h"

std::string getexepath()
{
	WCHAR result[MAX_PATH];
	char result_c[MAX_PATH];
	size_t num;

	GetModuleFileName(NULL, result, MAX_PATH);
	wcstombs_s(&num, result_c, result, MAX_PATH);

	auto ret = std::string(result_c);
	size_t pos = ret.find_last_of("/\\") + 1;
	ret = ret.substr(0, pos);

	return ret;
}

std::vector<byte> Utilities::ReadData(std::string path)
{
	std::string dataPath = getexepath();

	std::stringstream pathBuf;
	pathBuf << dataPath;
	pathBuf << path;

	std::vector<byte> Text;
	std::fstream file(pathBuf.str(), std::ios::in | std::ios::ate | std::ios::binary);

	if (file.is_open()) {
		Text.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(&Text[0]), Text.size());
		file.close();
	}

	return Text;
}