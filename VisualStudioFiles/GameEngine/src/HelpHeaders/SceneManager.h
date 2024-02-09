#pragma once

#pragma region Components
#include "Components/Camera.h"
#include "Components/Shader.h"
#include "Components/MeshData.h"
#include "Components/Transform.h"

#include "Components/ParentChild.h"
#include "Components/Particle.h"

#include "Components/ParticleGroup.h"

#include "Light/DirectionalLight.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#pragma endregion

#include "EntityTabler.h"
#include "SystemsManager.h"

#pragma region Systems

#include "../HelpHeaders/Systems/WindowManagerSystem.h"
#include "../HelpHeaders/Systems/InputHandlingSystem.h"
#include "../HelpHeaders/Systems/ImguiUISystem.h"

#include "../HelpHeaders/Systems/MeshLoadingSystem.h"

#include "../HelpHeaders/Systems/CameraHandelingSystem.h"

#include "../HelpHeaders/Systems/ShaderPropertiesSystem.h"
#include "../HelpHeaders/Systems/MeshRenderingSystem.h"

#include "../HelpHeaders/Systems/SerializationSystem.h"

#include "../HelpHeaders/Systems/ParticlesManagerSystem.h"

#include "../HelpHeaders/Systems/ExitToApplicationSystem.h"

#include "../HelpHeaders/Systems/DebugDrawSystem.h"

#pragma endregion

class SceneManager {

public:

	SceneManager() {
		curScene = new Scene();
		Scene::curScene = curScene;

		System::curScene = curScene;
	}

	~SceneManager() {

		std::cout << "DESTROYED SCENE MANAGER. " << std::endl;

		transSet->~CompSparseSet();
		shaderSet->~CompSparseSet();
		meshDataSet->~CompSparseSet();
		camSet->~CompSparseSet();

		parentChild->~CompSparseSet();
		partSet->~CompSparseSet();
		partiGroupSet->~CompSparseSet();

		ptLSet->~CompSparseSet();
		sptlSet->~CompSparseSet();
		drlSet->~CompSparseSet();

		curScene->~Scene();
	}

	void CreateCompSets();
	void PrintSystemNames();

	void ReSetCurScene() {
		Scene::curScene = curScene;
		System::curScene = curScene;
	}

	void SetCompSetsIndex() {
		for (unsigned int i = 0; i < curScene->sceneTabler.allCmpsSSets.size(); i++)
		{
			curScene->sceneTabler.allCmpsSSets[i]->SetCompSetIndex(i);
		}
	}

	virtual void CreateAppCompSets() = 0;
	virtual void PrintAppSystemNames() = 0;

private:

	Scene* curScene;

#pragma region Components
	CompSparseSet<Transform>* transSet;
	CompSparseSet<Shader>* shaderSet;
	CompSparseSet<MeshData>* meshDataSet;
	CompSparseSet<CameraComponent>* camSet;

	CompSparseSet<ParentChild>* parentChild;
	
	CompSparseSet<PointLight>* ptLSet;
	CompSparseSet<SpotLight>* sptlSet;
	CompSparseSet<DirectionalLight>* drlSet;

	CompSparseSet<Particle>* partSet;
	CompSparseSet<ParticleGroup>* partiGroupSet;
#pragma endregion

#pragma region Systems

	WindowManagerSystem windowManager;					//0
	InputHandlingSystem inputHandler;					//1
	ImguiUISystem imguiUISystem;						//2

	ParticlesManagerSystem particleManagerSystem;		//3
	MeshLoadingSystem meshLoader;						//4

	CameraHandelingSystem cameraUpdater;				//5

	ShaderPropertiesSystem shaderPropertySetter;		//6
	MeshRenderingSystem meshRenderer;					//7

	SerializationSystem serializerSystem;				//8

	DebugDrawSystem debugDraw;							//9

	ExitToApplicationSystem engineExit;					//10

#pragma endregion

};