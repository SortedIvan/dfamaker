#pragma once
#include "../hpp/file.hpp"
#include "../hpp/json.hpp"

using json = nlohmann::json;

struct TransitionObjectDto {
	int id;
	int transitionTo;
	int transitionFrom;
	bool isAssigned;
	bool isSelfLoop;
	std::vector<std::string> symbols;
};

struct StateDto {
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


class FileSystem {
private:
	File openedFile;
	bool fileIsLoaded = false;
	bool fileIsSaved = false;
	
public:
	FileSystem();
	void LoadFile(std::string path);
	void SaveLoadedFile();
	void SaveNewFile();
	std::vector<StateDto> LoadStateDtoObjects(json& data);
};