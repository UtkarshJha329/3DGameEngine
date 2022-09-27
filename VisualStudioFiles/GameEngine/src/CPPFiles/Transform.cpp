#include "../HelpHeaders/Components/Transform.h"

glm::vec3 Transform::worldFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Transform::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Transform::worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

void Transform::SerializeComponent()
{
	SFvecs(position, rotation, scale);
	//SerializeGlmVec3(rotation);
	//SerializeGlmVec3(scale);
}

void Transform::DeSerializeComponent()
{
	DSFvecs(&position, &rotation, &scale);
}

void Transform::UIfyComponent()
{
	UIfyVec3(position, rotation, scale);
}