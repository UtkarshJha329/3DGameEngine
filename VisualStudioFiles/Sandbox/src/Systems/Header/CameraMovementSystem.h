#pragma once

#include "SystemsManager.h"
#include "../../Components/Headers/CameraMovement.h"

class CameraMovementSystem : public System {

public:

	CameraMovementSystem() {
		systemName = typeid(CameraMovementSystem).name();
	}

	void UnorderedInit() override {}
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

};