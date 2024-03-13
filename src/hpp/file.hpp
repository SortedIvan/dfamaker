#pragma once
#include <string>

class File{
private:
	std::string fileName;
	std::string path;

public:
	File(std::string fileName, std::string path);
	File();
	std::string GetFileName();
	std::string GetPath();
};