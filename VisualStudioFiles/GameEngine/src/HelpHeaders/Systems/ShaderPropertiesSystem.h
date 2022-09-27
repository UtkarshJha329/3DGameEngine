#pragma once

#include "../SystemsManager.h"
#include "../ShaderUtils.h"

#include "../Components/Particle.h"

class ShaderPropertiesSystem : public System {

public:

	ShaderPropertiesSystem();

	void UnorderedInit() override;
	void PreAwake() override {}
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;


private:

	void SetPropInShaders();
	void UpdateRegularPropertiesInShader();
	void UpdateLightProperties(Shader* curShader);
	glm::mat4* WorldTransform(glm::mat4* childTrans, unsigned int childEntityID);

};
