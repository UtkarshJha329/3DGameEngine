#include "../../HelpHeaders/Systems/ObjectCreationSystem.h"

#include <filesystem>

void ObjectCreationSystem::PreAwake() 
{
	runPASOnReload = false;
	CreateObjects();
}

void ObjectCreationSystem::Awake(){}

void ObjectCreationSystem::CreateObjects() 
{
	auto transSet = curScene->GetCompSparseSet<Transform>();
	auto cameraSet = curScene->GetCompSparseSet<CameraComponent>();

	auto meshSet = curScene->GetCompSparseSet<MeshData>();
	auto shaderSet = curScene->GetCompSparseSet<Shader>();
	auto ptLSet = curScene->GetCompSparseSet<PointLight>();
	auto sptLSet = curScene->GetCompSparseSet<SpotLight>();
	auto drLSet = curScene->GetCompSparseSet<DirectionalLight>();

	//std::string curFolder = "D:/GitKrakenRepos/3DGameEngineProject/VisualStudioFiles/GameEngine/src/";

	//unsigned int cameraEntity = curScene->NewEntity();
	//transSet->Add(cameraEntity);
	//cameraSet->Add(cameraEntity);

	//curScene->GetCompOfEntity<Transform>(cameraEntity)->position = glm::vec3(20.0f, 20.0f, 0.0f);
	//curScene->GetCompOfEntity<Transform>(cameraEntity)->rotation = glm::vec3(0.0f, 90.0f, 0.0f);

	//std::string vertexShaderPath = curFolder + "ObjectShaders/TexturedLit/TexturedLit.vert";
	//std::string fragShaderPath = curFolder + "ObjectShaders/TexturedLit/TexturedLit.frag";

	//unsigned int sponzaEntity = curScene->NewEntity();
	//transSet->Add(sponzaEntity);
	//meshSet->Add(sponzaEntity);

	//std::string crysis_nano_suit_Model = curFolder + "Models/crysis_nano_suit_2/scene.gltf";

	//unsigned int crysisEntity = curScene->NewEntity();
	//std::cout << "CRISIS ENTITY: " << crysisEntity << std::endl;
	//meshSet->Add(crysisEntity);
	//shaderSet->Add(crysisEntity);
	//transSet->Add(crysisEntity);

	//curScene->GetCompOfEntity<MeshData>(crysisEntity)->meshPath = crysis_nano_suit_Model;
	//curScene->GetCompOfEntity<MeshData>(crysisEntity)->meshDataDir = "";

	//curScene->GetCompOfEntity<Transform>(crysisEntity)->position = glm::vec3(1.0f, 0.0f, 0.0f);
	//curScene->GetCompOfEntity<Transform>(crysisEntity)->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	//curScene->GetCompOfEntity<Transform>(crysisEntity)->rotation = glm::vec3(0.0f, 90.0f, 0.0f);

	//std::string sponza_Model = curFolder + "Models/glTF-Sample-Models-master/glTF-Sample-Models-master/2.0/Sponza/glTF/Sponza.gltf";
	//shaderSet->Add(sponzaEntity);

	//curScene->GetCompOfEntity<MeshData>(sponzaEntity)->meshPath = sponza_Model;
	//curScene->GetCompOfEntity<MeshData>(sponzaEntity)->meshDataDir = "";

	//curScene->GetCompOfEntity<Transform>(sponzaEntity)->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	//curScene->GetCompOfEntity<Transform>(sponzaEntity)->position = glm::vec3(0.0f, 0.0f, 2.5f);
	//curScene->GetCompOfEntity<Transform>(sponzaEntity)->scale = glm::vec3(0.1f, 0.1f, 0.1f);

	//glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
	//unsigned int ptLightE1 = curScene->NewEntity();
	//transSet->Add(ptLightE1);
	//ptLSet->Add(ptLightE1);
	//curScene->GetCompOfEntity<Transform>(ptLightE1)->position = glm::vec3(-1.0f, 0.0f, 20.0f);
	//curScene->GetCompOfEntity<PointLight>(ptLightE1)->diffuse = green;

	//unsigned int ptLightE2 = curScene->NewEntity();
	//transSet->Add(ptLightE2);
	//ptLSet->Add(ptLightE2);
	//curScene->GetCompOfEntity<Transform>(ptLightE2)->position = glm::vec3(-1.0f, 0.0f, 10.0f);
	//curScene->GetCompOfEntity<PointLight>(ptLightE2)->diffuse = green;

	//unsigned int ptLightE3 = curScene->NewEntity();
	//transSet->Add(ptLightE3);
	//ptLSet->Add(ptLightE3);
	//curScene->GetCompOfEntity<Transform>(ptLightE3)->position = glm::vec3(-1.0f, 0.0f, 0.0f);
	//curScene->GetCompOfEntity<PointLight>(ptLightE3)->diffuse = green;

//	unsigned int ptLightE4 = curScene->NewEntity();
	//transSet->Add(ptLightE4);
	//ptLSet->Add(ptLightE4);
	//curScene->GetCompOfEntity<Transform>(ptLightE4)->position = glm::vec3(-1.0f, 0.0f, -10.0f);
	//curScene->GetCompOfEntity<PointLight>(ptLightE4)->diffuse = green;

	//unsigned int ptLightE5 = curScene->NewEntity();
	//transSet->Add(ptLightE5);
	//ptLSet->Add(ptLightE5);
	//curScene->GetCompOfEntity<Transform>(ptLightE5)->position = glm::vec3(-1.0f, 0.0f, -20.0f);
	//curScene->GetCompOfEntity<PointLight>(ptLightE5)->diffuse = green;

	//unsigned int dirLightE = curScene->NewEntity();
	//transSet->Add(dirLightE);
	//drLSet->Add(dirLightE);
	//curScene->GetCompOfEntity<Transform>(dirLightE)->position = glm::vec3(1.0f, 1.0f, 10.0f);
	//curScene->GetCompOfEntity<DirectionalLight>(dirLightE)->direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	//curScene->GetCompOfEntity<DirectionalLight>(dirLightE)->diffuse = glm::vec3(0.7f, 0.7f, 0.8f);

	CreateAppObjects();
}