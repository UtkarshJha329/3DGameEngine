#pragma once

#include "../SystemsManager.h"
#include "../EntityTabler.h"

#include "../Components/Input.h"

#include "../Component.h"

class SerializationSystem : public System {

public:

	SerializationSystem() {
		systemName = typeid(SerializationSystem).name();
	}

	void UnorderedInit() override;
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

private:
	
	void LoadToTabler(Tabler* curTab, bool onlyFill);

	void DumpCurSaveStrToFile();
};