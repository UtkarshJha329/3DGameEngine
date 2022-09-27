#pragma once

#include "../SystemsManager.h"

#include "../Components/MeshData.h"
#include "../Components/Transform.h"
#include "../Components/Shader.h"

#include "../Components/ParentChild.h"
#include "../Components/Particle.h"
#include "../Components/ParticleGroup.h"

#include "../Components/Camera.h"
#include "../ShaderUtils.h"

class MeshRenderingSystem : public System {

public:
	MeshRenderingSystem();

	void UnorderedInit() override;
	void PreAwake() override {}
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

private:

	void DrawMeshTotal(MeshData* curMeshData, Shader* curShader, int entity = -1);
	void DrawParticleTotal(MeshData* curMeshData, Shader* curShader, int textureIndex, int entity = -1);
	void BindTexturesWithIndecies(MeshData* curMeshdata, Shader* parShader);
	void DrawMeshElements(MeshData* meshdata, Shader* parShader);
	void DrawMeshElementsBindingGivenTextureIndex(MeshData* meshdata, Shader* parShader, int textureIndex);

	void DrawMeshesPassOne(CameraComponent* curCam);

	void DrawMeshBackDepthPass(CameraComponent* curCam);
	void DrawMeshFrontDepthPass(CameraComponent* curCam);
	void DrawMeshTotalPlain(MeshData* curMeshData, Shader* curShader);
	void DrawMeshElementsPlain(MeshData* meshdata, Shader* parShader);

	glm::mat4 WorldTransform(glm::mat4* childTrans, unsigned int childEntityID);
};
