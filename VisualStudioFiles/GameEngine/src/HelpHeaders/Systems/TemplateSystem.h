#pragma once

#include "../SystemsManager.h"
#include "../EntityTabler.h"

class TemplateSystem : public System {

public:

	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

};