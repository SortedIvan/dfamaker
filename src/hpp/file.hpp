#pragma once
#include <string>
#include <vector>
#include <map>
#include "SFML/Graphics.hpp"

struct TransitionObjectDto 
{
	int id;
	int transitionTo;
	int transitionFrom;
	bool isAssigned;
	bool isSelfLoop;
	std::vector<char> symbols;
};


struct StateDto 
{
	std::string label;
	int stateId;
	std::vector<TransitionObjectDto> transitionObjects;
	std::map<char, int> transitions;
	std::vector<int> incomingTransitions;
	std::vector<int> outgoingTransitions;
	bool isAccepting;
	bool isStarting;
	sf::Vector2f statePosition;
	sf::Vector2f stateCenter;
};


class DfaFile
{
	private:
		std::string path;
		std::string filename;
		bool automaticStateLabels;
		int automaticStateLabelCount;
		int stateCounter;
		int transitionCounter;
		std::vector<std::string> inputStrings;
		std::vector<StateDto> states;

	public:
		DfaFile();
		DfaFile(std::string fileName, std::string path, bool automaticStateLabels, int automaticStateLabelCount, int stateCounter,
			int transitionCounter, std::vector<std::string>& inputStrings, std::vector<StateDto>& states);
		std::string GetFileName();
		std::string GetPath();
		bool GetAutomaticStateLabels();
		int GetAutomaticStateLabelCount();
		int GetStateCounter();
		int GetTransitionCounter();
		std::vector<std::string> GetInputStrings();
		std::vector<StateDto> GetStates();
};