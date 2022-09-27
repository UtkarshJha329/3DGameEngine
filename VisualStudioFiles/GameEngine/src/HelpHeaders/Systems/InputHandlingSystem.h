#pragma once

#include "../SystemsManager.h"
#include "../EntityTabler.h"

#include "../Components/Input.h"
#include "../WindowProperties.h"

class InputHandlingSystem : public System {

public:

	InputHandlingSystem() {
		systemName = typeid(InputHandlingSystem).name();
	}

	void UnorderedInit() override {}
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


private:

	void ResetInputs();
	void AddEnumToMaps();
};