#pragma once

#include <string>
#include <fstream>

#include <any>
#include <unordered_map>

#include <iostream>
#include <filesystem>

class RandomProps {

public:

	inline static std::string saveFileName = "Assets/Scenes/SaveFile.txt";
	inline static std::ofstream curOutFileStream;
	inline static std::ifstream curInFileStream;
	inline static std::string curSaveStr = "";
	inline static std::string curReadStr = "";

	inline static std::string curFolderPath = "D:/GitKrakenRepos/3DGameEngineProject/VisualStudioFiles/Executable/Assets/";
	inline static std::string assetsDirectoryPathString = "Assets";
	inline static std::filesystem::path assetsDirectoryPath = "Assets";

	inline static std::unordered_map<std::string, std::string> curCompDeserializationMap;

	inline static unsigned int particleEntity = 0;

	inline static bool createNewEntitySelectable = false;

};