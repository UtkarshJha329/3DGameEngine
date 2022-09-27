#pragma once

#define HAS_ANY_COMP 0
#define HAS_ALL_COMP 1

#include <vector>
#include <queue>

#include "EntityTabler.h"

class Archetype {

public:

	~Archetype() {
		compSparseIdsNdPriority.clear();
	}

public:

	std::vector<unsigned int> compSparseIdsNdPriority;
	
	unsigned int priorityType = HAS_ALL_COMP;
};

class System;

class SystemsManager {

public:

	std::vector<System*> systems;
	unsigned int systemBeginInt;

	System* engineExitSystem;

	std::vector<unsigned int> systemsTraversOrderIndex;

	unsigned int curSmallestSortedSetSize = 0;

	unsigned int firstApplicationSystemIndex = 0;

	bool firstAppSysRec = false;

	void ClearSystemsManager() {

		std::vector<System*>().swap(SystemsManager::systems);
	}

	void RunUnorderedInit();

	void RunSystemsPreAwake();
	void RunSystemsAwake();
	void RunSystemsStart();

	void RunSystemsUpdate();
	void RunSystemsLateUpdate();
	void RunSystemsClear();

	inline static SystemsManager* Instance() {
		if (!inst) {
			inst = new SystemsManager();
		}
		return inst;
	}

	void SetInstance() {
		inst = this;
	}

private:

	inline static SystemsManager* inst;

	void MakeTraversalOrder();

	void SortCompSet(Archetype archetype);

	void PrintTraversalOrder() {

		for (int i = 0; i < systemsTraversOrderIndex.size(); i++)
		{
			std::cout << systemsTraversOrderIndex[i] << std::endl;
		}
		std::cout << std::endl;
	}
};

class System {

public:

	System() {
		RegSys();
	}

	~System() {
		std::cout << "DESTRUCTOR OF SYSTEM CALLED." << std::endl;
	}

	Archetype archetype;

	unsigned int systemID;
	std::string systemName = " ";

	unsigned int curSmallestSetSize = 0;

	inline static Scene* curScene;

	void RegSys() {

		systemID = SystemsManager::Instance()->systems.size();
		std::cout << "NEW SYSTEM CREATED WITH ID: " << systemID << std::endl;
		SystemsManager::Instance()->systems.push_back(this);
		nxtSystemsIndecies.clear();
	}

	virtual void UnorderedInit() = 0;
	virtual void PreAwake() = 0;
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Clear() = 0;

	std::vector<unsigned int> nxtSystemsIndecies;

};
