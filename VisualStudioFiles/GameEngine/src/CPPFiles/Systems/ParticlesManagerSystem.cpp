#include "../../HelpHeaders/Systems/ParticlesManagerSystem.h"

void ParticlesManagerSystem::UnorderedInit()
{
	archetype.priorityType = HAS_ALL_COMP;
	archetype.compSparseIdsNdPriority.push_back(ParticleGroup::compSetIndex);

	systemName = typeid(ParticlesManagerSystem).name();
}

void ParticlesManagerSystem::PreAwake() {

}

void ParticlesManagerSystem::Awake() {}

void ParticlesManagerSystem::Start() {

}

void ParticlesManagerSystem::Update() {

	SpawnParticles();
	UpdateParticles();
	LoopParticles();
}

void ParticlesManagerSystem::LateUpdate() {

}

void ParticlesManagerSystem::Clear() {

}

void ParticlesManagerSystem::SpawnParticles()
{
	auto particleGroupSet = curScene->GetCompSparseSet<ParticleGroup>();
	auto partiSet = curScene->GetCompSparseSet<Particle>();

	auto transSet = curScene->GetCompSparseSet<Transform>();
	auto parChiSet = curScene->GetCompSparseSet<ParentChild>();

	for (int i = 0; i < particleGroupSet->DenseSize(); i++)
	{
		if (particleGroupSet->denseTArray[i].timeSinceLastSpawn >= 1.0f
			&& particleGroupSet->denseTArray[i].numberOfParticlesSpawned < particleGroupSet->denseTArray[i].totalNumberOfParticles) {

			Transform* groupTrans = curScene->GetCompOfEntity<Transform>(particleGroupSet->denseTArray[i].pairedEntity);

			for (int j = 0; j < particleGroupSet->denseTArray[i].numberOfParticlesPerSecond
				&& particleGroupSet->denseTArray[i].numberOfParticlesSpawned < particleGroupSet->denseTArray[i].totalNumberOfParticles; j++)
			{
			
				//GENERATES 2 PARTICLES IF ONLY 1 PARTICLE SHOULD BE CREATED WHILE LOOPING, BECAUSE LOOPING TIME IS -0.1S.
				unsigned int curParticleEntity = 0;
				if (particleGroupSet->denseTArray[i].deadEntitiesList.size() > 0) {

					int index = particleGroupSet->denseTArray[i].deadEntitiesList.front();
					particleGroupSet->denseTArray[i].deadEntitiesList.pop();
					
					curParticleEntity = particleGroupSet->denseTArray[i].particleEntities[index];

					particleGroupSet->denseTArray[i].particleAlive[index] = true;
					particleGroupSet->denseTArray[i].particleAliveTime[index] = 0.0f;
					//std::cout << "REUSED PARTICLE." << std::endl;

				}
				else {
					curParticleEntity = curScene->sceneTabler.entity_Set.NewEntity(false);
					partiSet->Add(curParticleEntity, false);

					transSet->Add(curParticleEntity, false);
					parChiSet->Add(curParticleEntity, false);

					particleGroupSet->denseTArray[i].particleEntities.push_back(curParticleEntity);
					particleGroupSet->denseTArray[i].particleAliveTime.push_back(0.0f);
					particleGroupSet->denseTArray[i].particleAlive.push_back(true);
				}

				Particle* particle = curScene->GetCompOfEntity<Particle>(curParticleEntity);
				Transform* trans = curScene->GetCompOfEntity<Transform>(curParticleEntity);

				particle->particleAlive = true;
				trans->position = groupTrans->position;
				trans->scale = glm::vec3(particleGroupSet->denseTArray[i].scale);

				float xNeg = 1;
				float zNeg = 1;
				if (RandomNumberUtils::Instance()->RandomNumber() > 0.5f) {
					xNeg = -1;
				}
				if (RandomNumberUtils::Instance()->RandomNumber() > 0.5f) {
					zNeg = -1;
				}

				float x = RandomNumberUtils::Instance()->RandomNumber();
				float y = glm::sin(particleGroupSet->denseTArray[i].coneInverseAngle);
				float z = RandomNumberUtils::Instance()->RandomNumber();

				particle->velocity = glm::vec3(xNeg * x, y, zNeg * z);
				particle->velocity *= particleGroupSet->denseTArray[i].particleSpeed;
				
				particleGroupSet->denseTArray[i].numberOfParticlesSpawned++;
			}
			particleGroupSet->denseTArray[i].timeSinceLastSpawn = 0.0f;
		}
		else {
			particleGroupSet->denseTArray[i].timeSinceLastSpawn += Time::deltaTime;
		}

	}

}

void ParticlesManagerSystem::UpdateParticles()
{
	auto particleGroupSet = curScene->GetCompSparseSet<ParticleGroup>();

	for (int i = 0; i < particleGroupSet->DenseSize(); i++)
	{
		for (int j = 0; j < particleGroupSet->denseTArray[i].particleEntities.size(); j++)
		{
			if(particleGroupSet->denseTArray[i].particleAlive[j]) {

				particleGroupSet->denseTArray[i].particleAliveTime[j] += Time::deltaTime;
				if (particleGroupSet->denseTArray[i].particleAliveTime[j] >= particleGroupSet->denseTArray[i].particleLifeTime) {

					unsigned int curParticleEntity = particleGroupSet->denseTArray[i].particleEntities[j];

					Particle* particle = curScene->GetCompOfEntity<Particle>(curParticleEntity);
					particle->particleAlive = false;

					particleGroupSet->denseTArray[i].particleAlive[j] = false;
					particleGroupSet->denseTArray[i].deadEntitiesList.push(j);
				}
				else {
					unsigned int curParticleEntity = particleGroupSet->denseTArray[i].particleEntities[j];

					Particle* particle = curScene->GetCompOfEntity<Particle>(curParticleEntity);
					Transform* trans = curScene->GetCompOfEntity<Transform>(curParticleEntity);

					trans->position += particle->velocity * Time::deltaTime;
					//trans->PrintTransformMat();
				}
			}
		}
	}

}

void ParticlesManagerSystem::LoopParticles()
{
	auto particleGroupSet = curScene->GetCompSparseSet<ParticleGroup>();

	for (int i = 0; i < particleGroupSet->DenseSize(); i++)
	{
		if (particleGroupSet->denseTArray[i].numberOfParticlesSpawned >= particleGroupSet->denseTArray[i].totalNumberOfParticles
			&& particleGroupSet->denseTArray[i].timeSinceLastSpawn >= particleGroupSet->denseTArray[i].particleLifeTime - 0.1f
			&& particleGroupSet->denseTArray[i].loop) {

			particleGroupSet->denseTArray[i].numberOfParticlesSpawned = 0;
		}
	}


}
