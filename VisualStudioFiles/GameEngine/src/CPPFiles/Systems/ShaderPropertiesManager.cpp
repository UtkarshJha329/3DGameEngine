#include "../../HelpHeaders/Systems/ShaderPropertiesSystem.h"

#include "../../HelpHeaders/Components/Transform.h"
#include "../../HelpHeaders/Components/Shader.h"
#include "../../HelpHeaders/Components/Camera.h"
#include "../../HelpHeaders/Components/ParentChild.h"

#include "../../HelpHeaders/Light/PointLight.h"
#include "../../HelpHeaders/Light/SpotLight.h"
#include "../../HelpHeaders/Light/DirectionalLight.h"


void ShaderPropertiesSystem::UnorderedInit() {

	archetype.priorityType = HAS_ALL_COMP;
	archetype.compSparseIdsNdPriority.push_back(Shader::compSetIndex);
	archetype.compSparseIdsNdPriority.push_back(Transform::compSetIndex);

	systemName = typeid(ShaderPropertiesSystem).name();
}

void ShaderPropertiesSystem::Awake() {

	if (!curScene->tempDepthShad) {
		std::string curFolder = "D:/GitKrakenRepos/3DGameEngineProject/VisualStudioFiles/Executable/Assets/ObjectShaders/ShadowPass/";
		curScene->tempDepthShad = new Shader();
		InitShader(curScene->tempDepthShad, "Assets/ObjectShaders/ShadowPass/Shadow.vert", "Assets/ObjectShaders/ShadowPass/Shadow.frag");

		std::cout << "CREATED TEMP DEPTH SHADER" << std::endl;
	}

}

void ShaderPropertiesSystem::Start() {
	SetPropInShaders();
}

void ShaderPropertiesSystem::Update() {
	UpdateRegularPropertiesInShader();
}

void ShaderPropertiesSystem::LateUpdate()
{
}

void ShaderPropertiesSystem::Clear() {

}

void ShaderPropertiesSystem::SetPropInShaders() {

	CameraComponent* curCamera = curScene->GetCompOfEntity<CameraComponent>(CameraComponent::mainCameraEntity);
	Transform* curCameraTrans = curScene->GetCompOfEntity<Transform>(CameraComponent::mainCameraEntity);

	auto shadSet = curScene->GetCompSparseSet<Shader>();

	for (unsigned i = 0; i < curSmallestSetSize; i++)
	{
		Shader *curShader = &shadSet->denseTArray[i];
		//std::cout << curShader->ID << std::endl;
		UseShader(curShader);
		setUFloat(curShader, "mat.shininess", 64.0f);
		setUFloat(curShader, "emissionStrength", 0.5f);
		
		UpdateLightProperties(curShader);

		setMat4(curShader, "cameraTransformMat", curCameraTrans->CalculateTransformMatr());
		setMat4(curShader, "projection", curCamera->projection);
		setVec3(curShader, "viewerPos", curCameraTrans->position);
	}
}

void ShaderPropertiesSystem::UpdateRegularPropertiesInShader()
{
	Transform* curCameraTrans;
	//Transform* cameraTrans = GameObjectsManager::mainCamera->transform;
	CameraComponent* curCamera;

	auto shadSet = curScene->GetCompSparseSet<Shader>();
	auto transSet = curScene->GetCompSparseSet<Transform>();

	auto parentChild = curScene->GetCompSparseSet<ParentChild>();

	curCameraTrans = curScene->GetCompOfEntity<Transform>(CameraComponent::mainCameraEntity);
	curCamera = curScene->GetCompOfEntity<CameraComponent>(CameraComponent::mainCameraEntity);

	for (unsigned i = 0; i < curSmallestSetSize; i++)
	{
		int curEntity = shadSet->denseIndexArray[i];
		
		Shader* curShader = &shadSet->denseTArray[i];
		Transform* curTransform = &transSet->denseTArray[i];

		if (curShader->shadNum == ShaderNumber::TexturedLit)
		{
			UpdateLightProperties(curShader);

			//unsigned int curEntity = transSet->denseIndexArray[i];
			UseShader(curShader);
			setUBool(curShader, "calculateLightingAndNormals", true);
			setMat4(curShader, "cameraTransformMat", curCameraTrans->CalculateTransformMatr());
			setVec3(curShader, "viewerPos", curCameraTrans->position);

			setMat4(curShader, "projection", curCamera->projection);

			//ParentChild* parChi = &parentChild->denseTArray[parentChild->sparse_Indecies[curEntity]];
			ParentChild* parChi = &(parentChild->denseTArray[parentChild->sparse_Indecies[curEntity]]);
			bool hasParent = parChi->parEntity != curEntity;

			if (hasParent) {
				glm::mat4* trans = WorldTransform(&curTransform->CalculateTransformMatr(), curEntity);
				setMat4(curShader, "transform", *trans);
			}
			else {
				setMat4(curShader, "transform", curTransform->CalculateTransformMatr());

			}
		}
	}

	auto particleSet = curScene->GetCompSparseSet<Particle>();
	auto ptLightSet = curScene->GetCompSparseSet<PointLight>();

	for (unsigned i = 0; i < particleSet->DenseSize(); i++)
	{
		int curEntity = particleSet->denseIndexArray[i];

		unsigned int curShaderEntity = particleSet->denseTArray[i].shaderEntity;
		
		Shader* curShader = &shadSet->denseTArray[shadSet->sparse_Indecies[curShaderEntity]];
		Transform* curTransform = &transSet->denseTArray[transSet->sparse_Indecies[curEntity]];

		if (curShader->shadNum == ShaderNumber::TexturedLit)
		{
			//unsigned int curEntity = transSet->denseIndexArray[i];
			UseShader(curShader);
			setUBool(curShader, "calculateLightingAndNormals", false);
			setMat4(curShader, "cameraTransformMat", curCameraTrans->CalculateTransformMatr());
			setVec3(curShader, "viewerPos", curCameraTrans->position);

			setMat4(curShader, "projection", curCamera->projection);

		}
	}

	Shader* tempDepthShad = curScene->tempDepthShad;
	UseShader(tempDepthShad);
	setMat4(tempDepthShad, "cameraTransformMat", curCameraTrans->CalculateTransformMatr());
	setVec3(tempDepthShad, "viewerPos", curCameraTrans->position);

	setMat4(tempDepthShad, "projection", curCamera->projection);

}

void ShaderPropertiesSystem::UpdateLightProperties(Shader* curShader)
{
	auto ptlSet = curScene->GetCompSparseSet<PointLight>();
	for (unsigned int j = 0; j < ptlSet->aprntDenseSize; j++)
	{
		char ptLightStr[17];
		sprintf_s(ptLightStr, "pointLights[%i].", j);
		PointLight* ptLit = &ptlSet->denseTArray[j];

		unsigned int ptLightEntity = ptlSet->denseIndexArray[j];
		Transform* ptLightTrans = curScene->GetCompOfEntity<Transform>(ptLightEntity);

		setVec3(curShader, std::string(ptLightStr) + "position", ptLightTrans->position);

		setVec3(curShader, std::string(ptLightStr) + "ambientColour", ptLit->ambient);
		setVec3(curShader, std::string(ptLightStr) + "diffuseColour", ptLit->diffuse);
		setVec3(curShader, std::string(ptLightStr) + "specularColour", ptLit->specular);
		setUFloat(curShader, std::string(ptLightStr) + "radius", ptLit->radius);

		//glm::veccurShader->3 plPos = ptLightTrans->position;
		//glm::veccurShader->4 glPos = curCamera->projection * glm::inverse(curCameraTrans->CalculateTransformMatr()) * glm::vec4(plPos, 1.0f);
		//glm::vec3 ndc(glPos.x / glPos.w, glPos.y / glPos.w, glPos.z / glPos.w);

		//glm::vec2 screenCoord(ndc.x, ndc.y);
		//screenCoord = screenCoord * 0.5f + glm::vec2(0.5f);

		//char plScreenCoordStr[18];
		//sprintf_s(plScreenCoordStr, "plScreenCoord[%i]", j);

		//
		//curShader->setVec2(std::string(plScreenCoordStr), screenCoord);

	}

	auto splSet = curScene->GetCompSparseSet<SpotLight>();
	for (unsigned int j = 0; j < splSet->aprntDenseSize; j++)
	{
		char sptLightStr[16];
		sprintf_s(sptLightStr, "spotLights[%i].", j);

		SpotLight* spotLight = &splSet->denseTArray[j];
		unsigned int sptLitEntity = splSet->denseIndexArray[j];
		Transform* sptLitTrans = curScene->GetCompOfEntity<Transform>(sptLitEntity);

		setVec3(curShader, std::string(sptLightStr) + "position", sptLitTrans->position);

		setVec3(curShader, std::string(sptLightStr) + "direction", spotLight->direction);
		setVec3(curShader, std::string(sptLightStr) + "ambientColour", spotLight->ambient);
		setVec3(curShader, std::string(sptLightStr) + "diffuseColour", spotLight->diffuse);
		setVec3(curShader, std::string(sptLightStr) + "specularColour", spotLight->specular);
		setUFloat(curShader, std::string(sptLightStr) + "radius", spotLight->radius);
		setUFloat(curShader, std::string(sptLightStr) + "minAngleCos", spotLight->minHlfAngleCos);
		setUFloat(curShader, std::string(sptLightStr) + "maxAngleCos", spotLight->maxHlfAngleCos);

	}

	auto drlSet = curScene->GetCompSparseSet<DirectionalLight>();

	for (unsigned int j = 0; j < drlSet->aprntDenseSize; j++)
	{
		char dirLightStr[15];
		sprintf_s(dirLightStr, "dirLights[%i].", j);

		DirectionalLight* dirLight = &drlSet->denseTArray[j];

		//GameObjectsManager::gameObjectShaders[i]->setVec3(std::string(dirLightStr) + "].position", dirLight.transform.position);
		setVec3(curShader, std::string(dirLightStr) + "direction", dirLight->direction);

		setVec3(curShader, std::string(dirLightStr) + "ambientColour", dirLight->ambient);
		setVec3(curShader, std::string(dirLightStr) + "diffuseColour", dirLight->diffuse);
		setVec3(curShader, std::string(dirLightStr) + "specularColour", dirLight->specular);
	}

}

glm::mat4* ShaderPropertiesSystem::WorldTransform(glm::mat4* childTrans, unsigned int childEntityID)
{
	auto parentChild = curScene->GetCompSparseSet<ParentChild>();
	auto transSet = curScene->GetCompSparseSet<Transform>();

	//ParentChild* parChi = &parentChild->denseTArray[parentChild->sparse_Indecies[curEntity]];
	ParentChild* parChi = &(parentChild->denseTArray[parentChild->sparse_Indecies[childEntityID]]);
	bool hasParent = parChi->parEntity != childEntityID;

	glm::mat4* parentTrans = &curScene->GetCompOfEntity<Transform>(parChi->parEntity)->CalculateTransformMatr();

	if (hasParent) {
		glm::mat4* trans = &((*WorldTransform(parentTrans, parChi->parEntity)) * (*childTrans));
		return trans;
	}
	else {
		return childTrans;
	}


	//return nullptr;
}
