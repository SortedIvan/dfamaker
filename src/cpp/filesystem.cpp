#include "../hpp/filesystem.hpp";
#include "../hpp/json.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "SFML/Graphics.hpp"

using json = nlohmann::json;

DfaFile FileSystem::LoadFile(std::string path) 
{
	try
	{
		std::ifstream file(path);

		json data = json::parse(file);

		if (data.size() == 0) {
			return DfaFile();
		}

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

void FileSystem::SaveLoadedFile() 
{
	if (!fileIsLoaded) 
	{
		std::cout << "No file to save";
		return;
	}

	if (fileIsSaved) 
	{
		std::cout << "File already saved";
		return;
	}
}

void FileSystem::SaveNewFile() 
{

}

FileSystem::FileSystem() 
{

}

std::vector<StateDto> FileSystem::LoadStateDtoObjects(json& data)
{
	json states = data["states"];
	std::vector<StateDto> loadedStates;

	std::cout << states.size();

	for (auto& state : states) 
	{

		StateDto stateObj;

		stateObj.label = state["label"];
		stateObj.stateId = state["stateId"];
		std::vector<TransitionObjectDto> transObjects;
		LoadStateTransitionDtoObjects(state["transitionObjects"], transObjects);
		stateObj.transitionObjects = transObjects;

		auto transitionsJson = state["transitions"];

		if (transitionsJson.size() == 0) 
		{
			stateObj.transitions = std::map<char, int>();
		}
		else 
		{
			std::map<char, int> transitions;
			for (auto it = transitionsJson.begin(); it != transitionsJson.end(); ++it)
			{
				int value = it.key()[0];
				transitions.insert({ value, it.value() });
			}

			stateObj.transitions = transitions;
		}

		stateObj.incomingTransitions = state["incomingTransitions"].get<std::vector<int>>();
		stateObj.outgoingTransitions = state["outgoingTransitions"].get<std::vector<int>>();
		stateObj.isAccepting = state["isAccepting"];
		stateObj.isStarting = state["isStarting"];

		auto statePosition = state["statePosition"];

		std::cout << statePosition.dump();

		float statePosX = statePosition[0];
		float statePosY = statePosition[1];

		stateObj.statePosition = sf::Vector2f(statePosX, statePosY);

		auto stateCenter = state["stateCenter"];
		float stateCenterX = stateCenter[0];
		float stateCenterY = stateCenter[1];

		stateObj.stateCenter = sf::Vector2f(stateCenterX, stateCenterY);

		loadedStates.push_back(stateObj);
	}

	return loadedStates;
}

void FileSystem::LoadStateTransitionDtoObjects(json& data, std::vector<TransitionObjectDto>& transObjects) 
{
	for (const auto& transition : data) 
	{

		TransitionObjectDto transitionObj;
		transitionObj.id = transition["id"];
		transitionObj.transitionTo = transition["transitionTo"];
		transitionObj.transitionFrom = transition["transitionFrom"];
		transitionObj.isAssigned = transition["isAssigned"];
		transitionObj.isSelfLoop = transition["isSelfLoop"];
		std::vector<int> symbols = transition["symbols"].get<std::vector<int>>();

		for (int i = 0; i < symbols.size(); i++)
		{
			transitionObj.symbols.push_back((char)symbols[i]);
		}

		transObjects.push_back(transitionObj);
	}
}
