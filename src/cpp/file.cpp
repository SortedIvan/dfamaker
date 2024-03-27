#include "../hpp/file.hpp"
#include <string>
#include <iostream>


DfaFile::DfaFile(std::string fileName, std::string path, bool automaticStateLabels, int automaticStateLabelCount, int stateCounter,
	int transitionCounter, std::vector<std::string>& inputStrings, std::vector<StateDto>& states) {
	this->filename = fileName;
	this->path = path;
	this->automaticStateLabelCount = automaticStateLabelCount;
	this->stateCounter = stateCounter;
	this->transitionCounter = transitionCounter;
	this->inputStrings = inputStrings;
	this->states = states;
}

DfaFile::DfaFile() {

}

std::string DfaFile::GetFileName() {
	return filename;
}

std::string DfaFile::GetPath() {
	return path;
}

bool DfaFile::GetAutomaticStateLabels() {
	return automaticStateLabels;
}

int DfaFile::GetAutomaticStateLabelCount() {
	return automaticStateLabelCount;
}

int DfaFile::GetStateCounter() {
	return stateCounter;
}

int DfaFile::GetTransitionCounter() {
	return transitionCounter;
}

std::vector<std::string> DfaFile::GetInputStrings() {
	return inputStrings;
}

std::vector<StateDto> DfaFile::GetStates() {
	return states;
}
