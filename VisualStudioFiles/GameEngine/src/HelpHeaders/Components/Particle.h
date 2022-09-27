#pragma once

#include "../Component.h"

class Particle : public Component<Particle> {

public:

	int textureIndex = -1;

	std::string texturePath = "Assets/EditorResources/Icons/Billboards/White.png";

	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	bool particleAlive = true;

	inline static unsigned int meshEntity = 0;
	inline static unsigned int shaderEntity = 0;

	virtual void SerializeComponent() override 
	{
		SFstrs(texturePath);
	}
	virtual void DeSerializeComponent() override 
	{
		DSFstrs(&texturePath);
	}
	virtual void UIfyComponent() override 
	{
		std::string textureName = texturePath.substr(texturePath.find_last_of('/') + 1, texturePath.length());
		UIfyString(textureName);
	}
};
