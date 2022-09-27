#include "../../HelpHeaders/Systems/InputHandlingSystem.h"

void InputHandlingSystem::PreAwake() {

}

void InputHandlingSystem::Awake() {
    glfwSetKeyCallback(WindowProperties::instance->window, key_callback);
}

void InputHandlingSystem::Start() {

}

void InputHandlingSystem::Update() {

}

void InputHandlingSystem::LateUpdate() {

    ResetInputs();

}

void InputHandlingSystem::Clear() {
    Input::Get().keyDown.clear();
    Input::Get().mouseButtonDown.clear();
}

void InputHandlingSystem::AddEnumToMaps() {
    ResetInputs();
}

void InputHandlingSystem::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key < 0) {
        return;
    }

    KeyCode curKeyCode = static_cast<KeyCode>(key);

    if (action == GLFW_PRESS) {
        Input::keyDown[curKeyCode] = true;
    }
}

void InputHandlingSystem::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    MouseButtonCode curKeyCode = static_cast<MouseButtonCode>(button);

    if (action == GLFW_PRESS) {
        Input::mouseButtonDown[curKeyCode] = true;
    }

}


void InputHandlingSystem::ResetInputs() {

    for (unsigned int i = 0; i < Input::numKeyCodes; i++)
    {
        Input::keyDown[Input::keyCodesVec[i]] = false;
    }

    for (auto& it : Input::mouseButtonDown)
    {
        it.second = false;
    }
}