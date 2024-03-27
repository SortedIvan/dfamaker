#include "../hpp/filesystem.hpp";
#include "../hpp/json.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "SFML/Graphics.hpp"

using json = nlohmann::json;

DfaFile FileSystem::LoadFile(std::string path) {
	try
	{
		std::ifstream file(path);

		json data = json::parse(file);

		// First, get all of the variables

		std::string filename = data["filename"];
		bool automaticStateLabels = data["automaticStateLabels"];

		int automaticStateLabelCount = data["automaticStateLabelCount"];
		int stateCounter = data["stateCounter"];
		int transitionCounter = data["transitionCounter"];
		std::vector<std::string> inputStrings = data["inputStrings"].get<std::vector<std::string>>();
		std::vector<StateDto> states = LoadStateDtoObjects(data);

		DfaFile dfaFile(filename, path, automaticStateLabels, automaticStateLabelCount, stateCounter, transitionCounter,
			inputStrings, states);


		return dfaFile;
	}
	catch (json::parse_error& ex)
	{
		std::cout << "There was an error loading/parsing the file: " << ex.what() << std::endl;
	}
}

void FileSystem::SaveLoadedFile() {
	if (!fileIsLoaded) {
		std::cout << "No file to save";
		return;
	}

	if (fileIsSaved) {
		std::cout << "File already saved";
		return;
	}
}

void FileSystem::SaveNewFile() {

}

FileSystem::FileSystem() {

}

std::vector<StateDto> FileSystem::LoadStateDtoObjects(json& data)
{
	json states = data["states"];
	std::vector<StateDto> loadedStates;

	std::cout << states.size();

	for (auto& state : states) {

		StateDto stateObj;

		stateObj.label = state["label"];
		stateObj.stateId = state["stateId"];
		std::vector<TransitionObjectDto> transObjects;
		LoadStateTransitionDtoObjects(state["transitionObjects"], transObjects);
		stateObj.transitionObjects = transObjects;

		auto transitionsJson = state["transitions"];
		std::map<char, int> transitions;
		for (auto it = transitionsJson.begin(); it != transitionsJson.end(); ++it) {
			transitions.insert({it.key()[0], it.value()});
		}

		stateObj.transitions = transitions;

		stateObj.incomingTransitions = state["incomingTransitions"].get<std::vector<int>>();
		stateObj.outgoingTransitions = state["outgoingTransitions"].get<std::vector<int>>();
		stateObj.isAccepting = state["isAccepting"];
		stateObj.isStarting = state["isStarting"];

		auto statePosition = state["statePosition"];
		float statePosX = statePosition["x"];
		float statePosY = statePosition["y"];

		stateObj.statePosition = sf::Vector2f(statePosX, statePosY);

		auto stateCenter = state["stateCenter"];
		float stateCenterX = stateCenter["x"];
		float stateCenterY = stateCenter["y"];

		stateObj.stateCenter = sf::Vector2f(stateCenterX, stateCenterY);

		loadedStates.push_back(stateObj);
	}

	return loadedStates;
}

void FileSystem::LoadStateTransitionDtoObjects(json& data, std::vector<TransitionObjectDto>& transObjects) {

	for (const auto& transition : data) {

		TransitionObjectDto transitionObj;
		transitionObj.id = transition["id"];
		transitionObj.transitionTo = transition["transitionTo"];
		transitionObj.transitionFrom = transition["transitionFrom"];
		transitionObj.isAssigned = transition["isAssigned"];
		transitionObj.isSelfLoop = transition["isSelfLoop"];
		std::vector<std::string> symbols = transition["symbols"].get<std::vector<std::string>>();

		for (int i = 0; i < symbols.size(); i++) {
			transitionObj.symbols.push_back(symbols[i][0]);
		}

		transObjects.push_back(transitionObj);
	}
}
