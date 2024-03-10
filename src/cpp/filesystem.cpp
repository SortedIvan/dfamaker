
#include "../hpp/filesystem.hpp";
#include "../hpp/json.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
using json = nlohmann::json;

void FileSystem::Test() {
	try
	{
		std::ifstream file("filestructure.json");
		json data = json::parse(file);
		std::cout << data["filename"];
	}
	catch (json::parse_error& ex)
	{
		std::cerr << "parse error at byte " << ex.what() << std::endl;
	}

}