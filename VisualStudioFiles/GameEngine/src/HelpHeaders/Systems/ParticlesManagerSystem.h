#pragma once

#include "../SystemsManager.h"
#include "../EntityTabler.h"

#include "../Components/Transform.h"
#include "../Components/ParentChild.h"

#include "../Components/Particle.h"
#include "../Components/ParticleGroup.h"

#include "../TimeVals.h"


class ParticlesManagerSystem : public System {

public:

	void UnorderedInit() override;
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

private:

	void SpawnParticles();
	void UpdateParticles();
	void LoopParticles();
};