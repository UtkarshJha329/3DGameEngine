#include "../../HelpHeaders/Systems/WindowManagerSystem.h"


void WindowManagerSystem::PreAwake() {}

void WindowManagerSystem::UnorderedInit() {

	archetype.priorityType = HAS_ALL_COMP;
	systemName = typeid(WindowManagerSystem).name();
}

void WindowManagerSystem::Awake() {

	SetupWindow(WindowProperties::instance->windName);
	EnableGLOptions();
	Input::gameWindow = WindowProperties::instance->window;
}

void WindowManagerSystem::Start() {

}

void WindowManagerSystem::Update() {

	ProcessInput();
	CursorVisibility();
}

void WindowManagerSystem::LateUpdate() {

}

void WindowManagerSystem::Clear() {

}

bool WindowManagerSystem::SetupWindow(std::string windowName) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	WindowProperties::instance->window = glfwCreateWindow(WindowProperties::instance->windowWidth, WindowProperties::instance->windowHeight, windowName.c_str(), NULL, NULL);
	if (WindowProperties::instance->window == NULL) {
		std::cout << "FAILED to create a GLFW window." << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(WindowProperties::instance->window);
	glfwSetFramebufferSizeCallback(WindowProperties::instance->window, Framebuffer_size_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(WindowProperties::instance->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "FAILED to initialize GLAD." << std::endl;
		return false;
	}

	WindowProperties::instance->windName = windowName;
	return true;
}

void WindowManagerSystem::ProcessInput()
{
	if (glfwGetKey(WindowProperties::instance->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(WindowProperties::instance->window, true);
	}
}

void WindowManagerSystem::CursorVisibility() {

	if (Input::cursorVisible) {
		glfwSetInputMode(WindowProperties::instance->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(WindowProperties::instance->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void WindowManagerSystem::EnableGLOptions()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
