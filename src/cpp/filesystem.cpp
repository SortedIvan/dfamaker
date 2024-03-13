
#include "../hpp/filesystem.hpp";
#include "../hpp/json.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "SFML/Graphics.hpp"


using json = nlohmann::json;

void FileSystem::LoadFile(std::string path) {
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
		//std::vector<StateDto> states = LoadStateDtoObjects(data);


		std::cout << data["filename"];


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
	
	for (const auto& state : states) {

	}
}


