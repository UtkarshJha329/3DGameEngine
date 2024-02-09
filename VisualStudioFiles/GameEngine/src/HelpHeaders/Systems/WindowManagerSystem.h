#pragma once

#include <stdio.h>

#include <iostream>
#include <string>

#include "../Components/Input.h"
#include "../SystemsManager.h"
#include "../../HelpHeaders/WindowProperties.h"

class WindowManagerSystem : public System {

public:
	
	WindowManagerSystem() {
		systemName = typeid(WindowManagerSystem).name();
	}

	bool SetupWindow(std::string windowName);

	void ProcessInput();

	static void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(WindowProperties::instance->viewPortX, WindowProperties::instance->viewPortY, width, height);
	}

	void EnableGLOptions();

	void UnorderedInit() override;
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

private:

	void CursorVisibility();

};
