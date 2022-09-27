#pragma once

#include "Component.h"

class WindowProperties {

public:

	float windowWidth = 1366;
	float windowHeight = 768;

	glm::vec2 viewportSize = { 1366, 768 };
	bool viewportResized = false;

	std::string windName = "Sora Engine";

	GLFWwindow* window = nullptr;
	const int viewPortX = 0.0f;
	const int viewPortY = 0.0f;

	/*inline virtual void SerializeComponent() override {
		SFstrs(windName);
		SF(DelimiterType::NEW_LINE, windowHeight, windowWidth);
	}

	inline virtual void DeSerializeComponent() override {
		DSFstrs(&windName);
		DSF(&windowHeight, &windowWidth);
	}*/


	inline static WindowProperties* instance;
};
