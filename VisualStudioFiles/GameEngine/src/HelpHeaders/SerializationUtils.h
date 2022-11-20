#pragma once

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RandomProps.h"

#define DSF(...) DeSerializeArgs(Split(#__VA_ARGS__, ','),__VA_ARGS__)
#define DSFvecs(...) DeSerializeGlmVec3(Split(#__VA_ARGS__, ','),__VA_ARGS__)
#define DSFstrs(...) DeSerializeString(Split(#__VA_ARGS__, ','),__VA_ARGS__)
#define DSFVecIntsT(name, x, size) DeSerializeVecIntsT(x, name, size)

#define SF(dt, ...) SerializeArgs(dt, Split(#__VA_ARGS__, ','),__VA_ARGS__)
#define SFvecs(...) SerializeGlmVec3(Split(#__VA_ARGS__, ','),__VA_ARGS__)
#define SFstrs(...) SerializeString(Split(#__VA_ARGS__, ','),__VA_ARGS__)
#define SFVecIntsT(x, name) SerializeVecIntsT(x, name)

std::vector<std::string> Split(std::string& str, std::string delim);

enum class DelimiterType {
	NONE,
	TAB_PER_WRITE,
	NEW_LINE,
	SPACE_PER_WRITE,
	COMMA_PER_WRITE
};

#pragma region Serialization

template<typename ...Args>
static void SerializeArgs(DelimiterType dt, std::vector<std::string> varNamesVec, Args... args) {

	std::string* varName = varNamesVec.data();

	for (auto p : { args... }) {

		SerializeArgs(dt, varName, p);
		varName++;
	}
}

template<typename ...Args>
static void SerializeArgs(DelimiterType dt, std::string* varName, Args... args) {

	if (varName) {
		RandomProps::curSaveStr += *varName + " : ";
	}

	for (auto p : { args... }) {

		RandomProps::curSaveStr += std::to_string(p);

		if (dt == DelimiterType::SPACE_PER_WRITE) {
			RandomProps::curSaveStr += " ";
		}
		else if (dt == DelimiterType::COMMA_PER_WRITE) {
			RandomProps::curSaveStr += ",";
		}
		else if (dt == DelimiterType::TAB_PER_WRITE) {
			RandomProps::curSaveStr += "\t";
		}
		else if (dt == DelimiterType::NEW_LINE) {
			RandomProps::curSaveStr += "\n";
		}
	}
}

static void SerializeGlmVec3(std::string* varName, glm::vec3 sVec) {

	SerializeArgs(DelimiterType::SPACE_PER_WRITE, varName, sVec.x, sVec.y, sVec.z);
	RandomProps::curSaveStr += "\n";
}

static void SerializeString(std::string* varName, std::string str) {

	RandomProps::curSaveStr += *varName + " : ";

	RandomProps::curSaveStr += str;

	RandomProps::curSaveStr += "\n";
}

template<typename... Vecs>
static void SerializeGlmVec3(std::vector<std::string> varNamesVec, Vecs... vecs) {

	std::string* varNames = varNamesVec.data();

	for (auto vec : { vecs... })
	{
		SerializeGlmVec3(varNames, vec);
		varNames++;
	}
}

template<typename T>
static void SerializeVecIntsT(std::vector<T>& vec, std::string name) {

	for (int i = 0; i < vec.size(); i++)
	{
		RandomProps::curSaveStr += name + " ";
		RandomProps::curSaveStr += std::to_string(i);
		RandomProps::curSaveStr += " : ";
		RandomProps::curSaveStr += std::to_string(vec[i]);
		RandomProps::curSaveStr += "\n";
	}

}

template<typename ...Strs>
static void SerializeString(std::vector<std::string> varNamesVec, Strs... strs) {

	std::string* varNames = varNamesVec.data();

	for (auto str : { strs... })
	{
		SerializeString(varNames, str);
		varNames++;
	}
}

#pragma endregion

#pragma region Deserialization

template<typename ...Args>
static void DeSerializeArgs(std::vector<std::string> varNamesVec, Args... args) {

	std::string* varName = varNamesVec.data();

	for (auto p : { args... }) {

		DeSerializeArgs(varName, p);
		varName++;
	}
}

template<typename ...Args>
static void DeSerializeArgs(std::string* varName, Args... args) {

	std::string serializedString = RandomProps::curCompDeserializationMap[*varName];

	std::vector<std::string> strs = Split(serializedString, ' ');

	int varIndex = 0;

	for (auto p : { args... }) {
	
		std::stringstream curVar(strs[varIndex]);
		curVar >> *p;

		varIndex++;
	}
}


static void LoadStringsIntoMap(std::string* readStr) {

	std::string serializedString = *readStr;

	std::vector<std::string> strs = Split(serializedString, " : ");

	RandomProps::curCompDeserializationMap[strs[0]] = strs[1];
}

static void DeSerializeGlmVec3(std::string* varName, glm::vec3* sVec) {

	DeSerializeArgs(varName, &(sVec->x), &(sVec->y), &(sVec->z));
}

static void DeSerializeString(std::string* varName, std::string* str) {

	DeSerializeArgs(varName, str);
}

template<typename... Vecs>
static void DeSerializeGlmVec3(std::vector<std::string> varNamesVec, Vecs... vecs) {

	std::string* varNames = varNamesVec.data();

	for (auto vec : { vecs... })
	{
		DeSerializeGlmVec3(varNames, vec);
		varNames++;
	}
}

template<typename ...Strs>
static void DeSerializeString(std::vector<std::string> varNamesVec, Strs... strs) {

	std::string* varNames = varNamesVec.data();

	for (auto str : { strs... })
	{
		DeSerializeString(varNames, str);
		varNames++;
	}
}


template<typename T>
static void DeSerializeVecIntsT(std::vector<T>& vec, std::string name, int size) {

	vec.clear();
	//std::cout << "Reading Size : " << RandomProps::curCompDeserializationMap.size() - ignore<< std::endl;
	for (int i = 0; i < size; i++)
	{
		T integer = (T)std::stoi(RandomProps::curCompDeserializationMap[name + " " + std::to_string(i)]);
		//std::cout << "INTEGER READ: " << integer << std::endl;
		vec.push_back(integer);
	}

}

#pragma endregion

static std::vector<std::string> Split(const std::string& str, char delim) {
	std::vector<std::string> strings;
	size_t start;
	size_t end = 0;
	while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
		end = str.find(delim, start);
		if (end == std::string::npos) {

			while (str.at(start) == ' ' || str.at(start) == '&' || str.at(start) == '_' || str.at(start) == '*') {
				start++;
			}

			strings.push_back(str.substr(start, str.length()));
			return strings;
		}
		while (str.at(start) == ' ' || str.at(start) == '&' || str.at(start) == '_' || str.at(start) == '*') {
			start++;
		}
		strings.push_back(str.substr(start, end - start));
	}
	return strings;
}

static std::vector<std::string> Split(std::string& str, std::string delim = " : " ) {
	std::vector<std::string> strings;
	size_t start;
	size_t end = 0;
	while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
		end = str.find(delim, start);
		if (end == std::string::npos) {

			while (str.at(start) == ' ' || str.at(start) == '&' || str.at(start) == '_' || str.at(start) == '*') {
				start++;
			}

			strings.push_back(str.substr(start, str.length()));
			return strings;
		}
		while (str.at(start) == ' ' || str.at(start) == '&' || str.at(start) == '_' || str.at(start) == '*') {
			start++;
		}
		strings.push_back(str.substr(start, end - start));
	}
	return strings;
}
