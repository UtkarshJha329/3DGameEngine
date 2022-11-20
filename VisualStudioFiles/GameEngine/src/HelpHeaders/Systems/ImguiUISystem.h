#pragma once

#include <string>

#include "../SystemsManager.h"

#include "../Components/Camera.h"

#include "../EntityTabler.h"
#include "../Components/ParentChild.h"
#include "../Components/MeshData.h"
#include "../EditorGuiVars.h"

#include "../RandomProps.h"

#include <unordered_set>
#include <filesystem>

class ImguiUISystem : public System {

public:

	ImguiUISystem() {
		systemName = typeid(ImguiUISystem).name();
	}

	void UnorderedInit() override {}
	void PreAwake() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void Clear() override;

private:

	void InitialiseImGUI();
	void PrepareImguiFrame();
	void DrawGUI();

	void DrawComponentsOfEntity(unsigned int entity);

	void DrawEntityTree(unsigned int entity, unsigned int* lastSelectedEntity);
	void DrawEntityTree(unsigned int entity, unsigned int* lastSelectedEntity, std::vector<bool>& drawnEntities);
	void DrawEntityTreeAll(unsigned int* lastSelectedEntityy, std::vector<bool>& shown);
	void MarkChildShownTrue(unsigned int curEntity, std::vector<bool>& shown);

private:

	unsigned int lastSelectedEntity = 0;
};