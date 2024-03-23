#pragma once
#include "../hpp/file.hpp"
#include "../hpp/json.hpp"
#include "SFML/Graphics.hpp"


using json = nlohmann::json;

class FileSystem {
private:
	bool fileIsLoaded = false;
	bool fileIsSaved = false;
	
public:
	FileSystem();
	DfaFile LoadFile(std::string path);
	void SaveLoadedFile();
	void SaveNewFile();
	std::vector<StateDto> LoadStateDtoObjects(json& data);
	void LoadStateTransitionDtoObjects(json& data, std::vector<TransitionObjectDto>& transObjects);
};