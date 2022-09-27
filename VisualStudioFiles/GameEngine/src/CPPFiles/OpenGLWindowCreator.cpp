#include "../HelpHeaders/OpenGlWindowCreator.h"

const int OpenGL_ND_WindowCreator::viewPortX = 0.0f;
const int OpenGL_ND_WindowCreator::viewPortY = 0.0f;

GLFWwindow* OpenGL_ND_WindowCreator::window;

float OpenGL_ND_WindowCreator::windowWidth = 1366;
float OpenGL_ND_WindowCreator::windowHeight = 768;

std::string OpenGL_ND_WindowCreator::windName;

bool OpenGL_ND_WindowCreator::SETUP_OPENGL_ND_WINDOW(std::string windowName) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(OpenGL_ND_WindowCreator::windowWidth, OpenGL_ND_WindowCreator::windowHeight, windowName.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "FAILED to create a GLFW window." << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "FAILED to initialize GLAD." << std::endl;
		return false;
	}

	OpenGL_ND_WindowCreator::windName = windowName;
	return true;
}

void OpenGL_ND_WindowCreator::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void OpenGL_ND_WindowCreator::EnableGLOptions()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
