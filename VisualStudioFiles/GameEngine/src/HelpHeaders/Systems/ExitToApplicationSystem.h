#pragma once

#include "../SystemsManager.h"

class ExitToApplicationSystem : public System {

public:
	
	ExitToApplicationSystem() {
		systemName = typeid(ExitToApplicationSystem).name();
	}


	void UnorderedInit() override {};
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

};