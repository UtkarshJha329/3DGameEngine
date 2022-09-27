#pragma once

#include "SystemsManager.h"

#include "../HelpHeaders/Systems/WindowManagerSystem.h"
#include "../HelpHeaders/Systems/InputHandlingSystem.h"
#include "../HelpHeaders/Systems/ImguiUISystem.h"

#include "../HelpHeaders/Systems/MeshLoadingSystem.h"

#include "../HelpHeaders/Systems/CameraHandelingSystem.h"

#include "../HelpHeaders/Systems/ShaderPropertiesSystem.h"
#include "../HelpHeaders/Systems/MeshRenderingSystem.h"

#include "../HelpHeaders/Systems/SerializationSystem.h"

#include "../HelpHeaders/Systems/ExitToApplicationSystem.h"


class SystemsOrderManager {

public:

	SystemsOrderManager() {

		//SystemsManager::Instance()->engineExitSystem = &engineExit;

	}

	SystemsOrderManager(const SystemsOrderManager&) = delete;

	void SetUpSystemsOrder();

	virtual unsigned int SetUpApplicationSystemsOrder() = 0;

private:

	//WindowManagerSystem windowManager;
	//InputHandlingSystem inputHandler;
	//ImguiUISystem imguiUISystem;
	//
	//MeshLoadingSystem meshLoader;
	//
	//CameraHandelingSystem cameraUpdater;
	//
	//ShaderPropertiesSystem shaderPropertySetter;
	//MeshRenderingSystem meshRenderer;
	//
	//SerializationSystem serializerSystem;
	//
	//ExitToApplicationSystem engineExit;
};