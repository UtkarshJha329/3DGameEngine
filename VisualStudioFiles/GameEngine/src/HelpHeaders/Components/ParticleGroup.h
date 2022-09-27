#pragma once

#include <vector>
#include <queue>

#include "../Component.h"

#include "Particle.h"

class ParticleGroup : public Component<ParticleGroup> {

public:

	int totalNumberOfParticles = 100;
	int numberOfParticlesPerSecond = 5;
	
	int numberOfParticlesSpawned = 0;

	float timeSinceLastSpawn = 1.0f;

	float particleLifeTime = 5.0f;

	float particleSpeed = 10.0f;
	float scale = 0.5f;
	bool affectedByGravity = false;

	float coneInverseAngle = glm::pi<glm::float32>() / 2;

	bool loop = true;

	int textureIndex = -1;
	std::string texturePath = "Assets/EditorResources/Icons/Billboards/White.png";

	std::vector<unsigned int> particleEntities;
	std::vector<bool> particleAlive;
	std::vector<float> particleAliveTime;
	std::queue<unsigned int> deadEntitiesList;


	virtual void SerializeComponent() override 
	{
		SF(DelimiterType::NEW_LINE, totalNumberOfParticles, numberOfParticlesPerSecond);
		SF(DelimiterType::NEW_LINE, particleLifeTime, particleSpeed, scale, coneInverseAngle);
		SF(DelimiterType::NEW_LINE, loop);
		SFstrs(texturePath);
	}
	virtual void DeSerializeComponent() override 
	{
		DSF(&totalNumberOfParticles, &numberOfParticlesPerSecond);
		DSF(&particleLifeTime, &particleSpeed, &scale, &coneInverseAngle);
		DSF(&loop);
		DSFstrs(&texturePath);
	}
	virtual void UIfyComponent() override 
	{
		UIfyInt(totalNumberOfParticles);
		UIfyInt(numberOfParticlesPerSecond);

		UIfyFloat(particleLifeTime);
		UIfyFloat(scale);
		UIfyFloat(coneInverseAngle);

		UIfyBool(loop);

		UIfyString(texturePath);
	}
};