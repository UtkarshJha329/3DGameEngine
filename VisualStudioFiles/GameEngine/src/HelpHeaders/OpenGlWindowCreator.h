#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <iostream>
#include <string>

#include "Components/Input.h"

class OpenGL_ND_WindowCreator {

public:
	OpenGL_ND_WindowCreator() = delete;

	static float windowWidth;
	static float windowHeight;

	static std::string windName;

	static GLFWwindow* window;
	static const int viewPortX;
	static const int viewPortY;

	static bool SETUP_OPENGL_ND_WINDOW(std::string windowName);

	static void ProcessInput();

	static void Framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(viewPortX, viewPortY, width, height);
	}

	static void EnableGLOptions();

private:

};
