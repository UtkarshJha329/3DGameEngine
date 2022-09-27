#pragma once

#include "../SystemsManager.h"

#include "../Components/Camera.h"

class CameraHandelingSystem : public System {

public:

	CameraHandelingSystem() {
		systemName = typeid(CameraHandelingSystem).name();
	}

	void UnorderedInit() override{}
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;


private:

	void UpdateCameraProjection();

	void CreateCameraFrameBuffers();
	void CreateCameraFrameBufSpecific(CameraComponent* curComp);

	void SetCameraFrameBuffers(float width, float height);

	void ClearCameraBuffers();
	void ClearCameraBufSpecific(CameraComponent* curComp);

};