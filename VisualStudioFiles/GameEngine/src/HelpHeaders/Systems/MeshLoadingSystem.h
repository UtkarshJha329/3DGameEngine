#pragma once

#include "../SystemsManager.h"
#include "../Components/MeshData.h"
#include "../Components/Transform.h"
#include "../Components/Shader.h"

#include "../Components/ParentChild.h"
#include "../Components/Particle.h"
#include "../Components/ParticleGroup.h"

#include "../ShaderUtils.h"
#include "../RandomNumber.h"

#include <unordered_map>

class MeshLoadingSystem : public System {

public:
	MeshLoadingSystem();

	void UnorderedInit() override;
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

private:

	void GenerateABOs(MeshData* meshData, GLsizeiptr bufferSize, Shader* curShader);

	void LoadModel(unsigned int curMeshEntity, Shader* curShader);

	void ProcessNode(const aiScene* scene, unsigned int curMeshEntity, std::string curMeshDir, Shader* curShader);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData* meshN, Shader* curShader, std::string meshDataDir);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, unsigned int meshEntity, Shader* curShader, std::string meshDataDir);
	std::vector<unsigned int> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName, std::string curMeshDataDir);

	int LoadParticleTextureToCommonMeshData(std::string path);

	void FreeStuff(MeshData* curMeshData);

	std::vector<std::vector<std::pair<size_t, std::string>>> loadedModelPaths;

};
