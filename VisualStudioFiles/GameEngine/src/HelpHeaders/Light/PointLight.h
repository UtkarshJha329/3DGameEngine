#pragma once

#include "../Component.h"

class PointLight : public Component<PointLight> {

public:

	glm::vec3 ambient = glm::vec3(0.1f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);

	float radius = 30.0f;

	static const std::string defVertShaderPath;
	static const std::string defFragShaderPath;

	virtual void SerializeComponent() override;
	virtual void DeSerializeComponent() override;
	virtual void UIfyComponent() override;
};