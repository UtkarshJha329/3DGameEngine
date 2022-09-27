#pragma once

#include "../SystemsManager.h"

#include "../EntityTabler.h"

#include "../Components/Transform.h"
#include "../Components/MeshData.h"
#include "../Components/Shader.h"
#include "../Components/Camera.h"

#include "../Light/PointLight.h"
#include "../Light/SpotLight.h"
#include "../Light/DirectionalLight.h"

class ObjectCreationSystem : public System {

public:

	void PreAwake() override;
	void Awake() override;
	void Start() override {}
	void Update() override {}
	void LateUpdate() override {}
	void Clear() override {}

private:

	void CreateObjects();

	virtual void CreateAppObjects() = 0;
};