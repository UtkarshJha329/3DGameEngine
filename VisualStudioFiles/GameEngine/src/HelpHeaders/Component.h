#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SerializationUtils.h"
#include "UIUtils.h"

struct BaseComponent {

private:

	static uint32_t lastCompID;
	static uint32_t NextPropertyId();
};

template<typename T>
class Component : public BaseComponent {

public:
	
	static const uint32_t ID;
	inline static std::string comp_name;

	static const uint32_t numComponents;

	unsigned int pairedEntity;

	static void ReadComponentSave() {

		while (true) {

			std::string curLine = "";
			std::getline(RandomProps::curInFileStream, curLine);
			//std::cout << "Deserialized Line : " << curLine << std::endl;
			if (strcmp(curLine.c_str(), "") == 0) {
				break;
			}
			LoadStringsIntoMap(&curLine);
		}
	}

	inline static int compSetIndex = -1;

	virtual void SerializeComponent() = 0;
	virtual void DeSerializeComponent() = 0;
	virtual void UIfyComponent() = 0;

};