#pragma once

#include "SystemsManager.h"

#include "TestSystem.h"
#include "Components/Camera.h"
#include "Components/Shader.h"
#include "Components/MeshData.h"
#include "Components/Transform.h"

class CompDestroySystem : public System {

public:

	CompDestroySystem() {
		systemName = typeid(CompDestroySystem).name();
	}

	void UnorderedInit() override {}
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

};