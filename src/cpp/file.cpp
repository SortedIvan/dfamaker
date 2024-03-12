#include "../hpp/file.hpp"
#include <string>

File::File(std::string fileName, std::string path) {
	this->fileName = fileName;
	this->path = path;
}

std::string File::GetFileName() {
	return fileName;
}

std::string File::GetPath() {
	return path;
}
