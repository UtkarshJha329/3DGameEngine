#pragma once

#include "Component.h"

class CameraMovementComponent : public Component<CameraMovementComponent> {

public:

	float cameraMvSpeed = 20.0f;
	float sensitivity = 3.0f;

	virtual void SerializeComponent() override;
	virtual void DeSerializeComponent() override;
	virtual void UIfyComponent() override;
};