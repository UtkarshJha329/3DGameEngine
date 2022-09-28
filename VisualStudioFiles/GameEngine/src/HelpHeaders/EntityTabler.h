#pragma once

#include "RandomProps.h"
#include "Component.h"

#include "RandomNumber.h"

#include <iostream>
#include <functional>

class Tabler;

class BaseSparseSet {

public:


	std::vector<unsigned int> sparse_Indecies;
	std::vector<unsigned int> denseIndexArray;
	std::vector<bool> serializable;

	virtual void Add(unsigned int spID, bool _serializable = true) = 0;
	virtual void Remove(unsigned int spID) = 0;
	virtual bool EntityHasComp(unsigned int spID) = 0;
	virtual bool Serializable(unsigned int spID) = 0;
	virtual void SwapDenseEntities(unsigned int spID1, unsigned int spID2) = 0;
	virtual unsigned int DenseSize() = 0;
	virtual void SerializeCompSet() = 0;
	virtual void DeSerializeCompSet(int numComps) = 0;
	virtual void ResetDenseTArray() = 0;

	virtual void UIfyComp(unsigned int spID) = 0;


	BaseSparseSet() {
		ClearDenseIndexArray();
	}

	~BaseSparseSet() {
		ClearDenseIndexArray();
	}

	void SetCompName(std::string name) {
		compName = name;
	}

	std::string compName;


	void ClearDenseIndexArray() {
		denseIndexArray.clear();
		std::vector<unsigned int>().swap(denseIndexArray);
	}

	virtual void SetCompSetIndex(unsigned int index) = 0;
	virtual void Reserve(unsigned int num) = 0;
	virtual void ReserveDense(unsigned int numEntities, unsigned int num) = 0;
	virtual size_t GetCompSize() = 0;
	virtual unsigned int GetAllSetVecIndex() = 0;
};

template<typename T>
class CompSparseSet : public BaseSparseSet{

public:

	inline static unsigned int allSetVecIndex;

	std::vector<T> denseTArray;
	
	unsigned int aprntDenseSize = 0;

	const size_t sizeofT = sizeof(T);

	CompSparseSet() {
		SetCompName(typeid(T).name());

		CompChangeDetector::Instance()->Add(compName, sizeof(T), this);

		std::cout << "CONSTRUCTOR OF COMP : " << compName << std::endl;

		ClearCompSetArrays();

		allSetVecIndex = Scene::curScene->sceneTabler.allCmpsSSets.size();
		Scene::curScene->sceneTabler.allCmpsSSets.push_back((BaseSparseSet*)this);
		T::compSetIndex = allSetVecIndex;
		T::comp_name = compName;
	}
	~CompSparseSet() {
		std::cout << "DESTRUCTOR OF COMP : " << compName << std::endl;
		ClearCompSetArrays();
	}
	CompSparseSet& operator=(CompSparseSet&)= delete;
	CompSparseSet(const CompSparseSet&)		= delete;

	inline virtual void SetCompSetIndex(unsigned int index) override {
		allSetVecIndex = index;
		T::compSetIndex = allSetVecIndex;
		SetCompName(typeid(T).name());
		T::comp_name = compName;
		std::cout << T::comp_name << " : " << T::compSetIndex << std::endl;
	}

	inline virtual size_t GetCompSize() override {
		return sizeof(T);
	}

	inline virtual unsigned int GetAllSetVecIndex() override {
		return allSetVecIndex;
	}

	void ClearCompSetArrays() {
		denseTArray.clear();
		sparse_Indecies.clear();

		std::vector<unsigned int>().swap(sparse_Indecies);
		std::vector<T>().swap(denseTArray);
	}

	virtual void Add(unsigned int spID, bool _serializable = true) override {

		Tabler* curSceneTabler = &(Scene::curScene->sceneTabler);

		if (aprntDenseSize < denseTArray.size()) {
			//std::cout << "REUSED COMPONENT for : " << spID << std::endl;
			sparse_Indecies[spID] = aprntDenseSize;
			denseIndexArray[aprntDenseSize] = spID;
			denseTArray[aprntDenseSize].pairedEntity = spID;
			serializable[aprntDenseSize] = _serializable;
		}
		else {
			//std::cout << "CREATED NEW COMPONENT for : " << compName << " : " << spID << " Size : " << sparse_Indecies.size() << std::endl;
			if (spID >= sparse_Indecies.size()) {
				sparse_Indecies.resize(spID + 20);
			}
			sparse_Indecies[spID] = denseTArray.size();
			denseTArray.emplace_back();
			denseIndexArray.emplace_back(spID);
			serializable.emplace_back(_serializable);
			denseTArray.back().pairedEntity = spID;
			//std::cout << "ADDED NEW COMP FOR ENTITY : " << spID << std::endl;
		}
		aprntDenseSize++;
	}

	virtual void Remove(unsigned int spID) override {

		if (aprntDenseSize > sparse_Indecies[spID] && denseIndexArray[sparse_Indecies[spID]] == spID) {
			//denseTArray.erase(denseTArray.begin() + sparse_Index[spID]);

			unsigned int curDenseIndex = sparse_Indecies[spID];
			unsigned int lastDenseVal = denseIndexArray[aprntDenseSize - 1];

			denseIndexArray[curDenseIndex] = lastDenseVal;

			denseTArray[curDenseIndex] = denseTArray[aprntDenseSize - 1];

			sparse_Indecies[lastDenseVal] = curDenseIndex;

			sparse_Indecies[spID]++;

			if (aprntDenseSize != 0) {
				aprntDenseSize--;
			}

			std::cout << "REMOVED ENTITY" << std::endl;
		}
		else {
			std::cout << "FAILED TO REMOVE: " << spID << std::endl;
			//std::cout << "DOES NOT HAVE THIS COMPONENT" << std::endl;
		}
	}

	bool EntityHasComp(unsigned int spID) override {
		Tabler* curSceneTabler = &(Scene::curScene->sceneTabler);

		return curSceneTabler->entity_Set.entity_Alive[spID]
				&& sparse_Indecies.size() > spID
				&& denseIndexArray.size() > sparse_Indecies[spID]
				&& denseIndexArray[sparse_Indecies[spID]] == spID;
	}

	bool Serializable(unsigned int spID) override {

		Tabler* curSceneTabler = &(Scene::curScene->sceneTabler);

		return EntityHasComp(spID)
			&& curSceneTabler->entity_Set.entity_serializable[spID]
			&& serializable.size() > sparse_Indecies[spID]
			&& serializable[sparse_Indecies[spID]];
	}

	void SwapDenseEntities(unsigned int entityID1, unsigned int entityID2) override{

		if (entityID1 == entityID2) {
			return;
		}

		unsigned int sparseVal1 = sparse_Indecies[entityID1];
		unsigned int sparseVal2 = sparse_Indecies[entityID2];

		T tempDesnse = denseTArray[sparseVal1];
		unsigned int tempEntityIndex = denseIndexArray[sparseVal1];

		denseTArray[sparseVal1] = denseTArray[sparseVal2];
		denseIndexArray[sparseVal1] = denseIndexArray[sparseVal2];

		sparse_Indecies[entityID2] = sparseVal1;

		denseIndexArray[sparseVal2] = tempEntityIndex;
		denseTArray[sparseVal2] = tempDesnse;
		sparse_Indecies[entityID1] = sparseVal2;

	}

	unsigned int DenseSize() {
		return aprntDenseSize;
	}

	virtual void Reserve(unsigned int num) override {
		sparse_Indecies.reserve(num);
		denseTArray.reserve(num);
		denseIndexArray.reserve(num);
	}

	virtual void ReserveDense(unsigned int numEntities, unsigned int num) override {
		sparse_Indecies.reserve(numEntities);
		denseTArray.reserve(num);
		denseIndexArray.reserve(numEntities);
	}

	virtual void ResetDenseTArray() override {
		denseTArray.clear();
		std::vector<T>(aprntDenseSize, T()).swap(denseTArray);
	}

	virtual void SerializeCompSet() override {

		for (unsigned int i = 0; i < aprntDenseSize; i++)
		{
			if (serializable[i]) {
				denseTArray[i].SerializeComponent();
				RandomProps::curSaveStr += "entity : " + std::to_string(denseIndexArray[i]) + "\n";
				RandomProps::curSaveStr += "\n";
			}
		}
	}

	virtual void DeSerializeCompSet(int numComps) override {

		//std::cout << "Dense Size : " << aprntDenseSize << std::endl;
		for (unsigned int i = 0; i < numComps; i++)
		{
			if (serializable[i]) {
				T::ReadComponentSave();

				int entity = std::stoi(RandomProps::curCompDeserializationMap["entity"]);

				denseTArray[sparse_Indecies[entity]].DeSerializeComponent();
				RandomProps::curCompDeserializationMap.clear();
			}
		}
	}

	virtual void UIfyComp(unsigned int spID) override {
		denseTArray[sparse_Indecies[spID]].UIfyComponent();
	}

};

class EntitySparseSet {

public:

	std::vector<unsigned int> sparse_Index;
	std::vector<unsigned int> dense_Entities;

	std::vector<bool> entity_Alive;
	std::vector<bool> entity_serializable;

	unsigned int filledDenseEntities = 0;

	unsigned int NewEntity(bool _serializable = true);

	void DeleteEntity(unsigned int entity);

	void Reserve(unsigned int num) {
		sparse_Index.reserve(num);
		dense_Entities.reserve(num);
	}

	EntitySparseSet() {

		std::cout << "ENTITY SPARSE SET CREATED." << std::endl;

		ClearEntitySparseSet();

	}
	~EntitySparseSet() {

		std::cout << "ENTITY SPARSE SET DESTROYED." << std::endl;

		ClearEntitySparseSet();
	}

	void ClearEntitySparseSet() {
		std::vector<unsigned int>().swap(sparse_Index);
		std::vector<unsigned int>().swap(dense_Entities);
		std::vector<bool>().swap(entity_Alive);
		std::vector<bool>().swap(entity_serializable);
	}

	EntitySparseSet& operator=(const EntitySparseSet&)	= delete;
	EntitySparseSet(const EntitySparseSet&)				= delete;

private:

	unsigned int freeHead;
	unsigned int available = 0;

};

class CompChangeDetector {

public:

	CompChangeDetector() = default;

	~CompChangeDetector() {
		std::cout << "DESTROYED COMP CHANGED DETECTOR. " << std::endl;
	}

	void Add(std::string& str, unsigned int size, BaseSparseSet* bs) {

		if (map.count(str) > 0) {

			if (map[str] != size) {
				std::cout << "COMPONENT " << str << " UPDATED." << std::endl;
				map[str] = size;
			}
		}
		else {
			std::cout << "NEW COMPONENT " << str << " WAS ADDED." << std::endl;
			bss.emplace_back(bs);
			map[str] = size;
		}
	}

	void CheckForChanges() {

		for (int i = 0; i < bss.size(); i++)
		{
			std::cout << bss[i]->compName /*<< " : " << ccd.get()->bss[i]->GetCompSize() */ << std::endl;

			unsigned int oldSize = map[bss[i]->compName];
			unsigned int newSize = bss[i]->GetCompSize();

			std::cout << "------>OLD SIZE : " << oldSize << std::endl;
			std::cout << "------>NEW SIZE : " << newSize << std::endl;

			if (oldSize != newSize) {
				changedComps.push_back(i);
				map[bss[i]->compName] = newSize;
			}

			//std::this_thread::sleep_for(0.2s);
		}
	}

	inline static CompChangeDetector* Instance() {
		return inst;
	}

	void SetInstance() {

		if (inst) {
			std::cout << "COMP CHANGE DETECTOR INSTANCE WAS OVERRIDEN." << std::endl;
		}
		inst = this;
	}

	std::unordered_map<std::string, unsigned int> map;
	std::vector<BaseSparseSet*> bss;

	std::vector<unsigned int> changedComps;

private:

	static CompChangeDetector* inst;

};

//template<typename T>
//CompSparseSet<T> Comp_Set;

class Tabler {

public:
	
	~Tabler() {
		entity_Set.~EntitySparseSet();
		DestComps();
	}

	EntitySparseSet entity_Set;

	std::vector<BaseSparseSet*> allCmpsSSets;

	/*
#pragma region EasyCompSetAccessFunc
	template<typename T>
	inline static void RsrvCompSize(unsigned int num) {
		Comp_Set<T>.Reserve(num);
	}

	template<typename T>
	inline static void AddCompToEntity(unsigned int entity) {
		Comp_Set<T>.Add(entity);
	}

	template<typename T>
	inline static void RmvCompFrmEntity(unsigned int entity) {
		Comp_Set<T>.Remove(entity);
	}
#pragma endregion
	*/

	void DestComps() {
		allCmpsSSets.clear();
		std::vector<BaseSparseSet*>().swap(allCmpsSSets);
	}

	template<typename T>
	CompSparseSet<T>* GetCompSparseSet(unsigned int index) {
		return std::static_pointer_cast<CompSparseSet<T>>(allCmpsSSets[index]);
	}

	void RmvEntity(unsigned int entity);

	void DebugPrint() {

		std::cout << "ENTITY NUMBER : SPARSE INDEX : DENSE VALUE" << std::endl;

		for (int i = 0; i < entity_Set.sparse_Index.size(); i++)
		{
			if (entity_Set.entity_Alive[i] == true) {
				unsigned int indexer = Tabler::entity_Set.sparse_Index[i];
				std::cout << i << "\t:\t";
				std::cout << indexer << "\t\t" << Tabler::entity_Set.dense_Entities[indexer] << std::endl;
			}
		}

	}
};

class Shader;

class Scene {

public:

	~Scene() {
		sceneTabler.~Tabler();
	}

	inline static Scene* curScene;
	Shader* tempDepthShad = nullptr;
	Tabler sceneTabler;

	template<typename T>
	CompSparseSet<T>* GetCompSparseSet() {
		return (CompSparseSet<T>*)(sceneTabler.allCmpsSSets[T::compSetIndex]);
	}
	
	unsigned int NewEntity() {
		return sceneTabler.entity_Set.NewEntity();
	}

	template<typename T>
	T* GetCompOfEntity(unsigned int entity) {

		CompSparseSet<T>* compSet = GetCompSparseSet<T>();
		if (compSet->EntityHasComp(entity))
			return &compSet->denseTArray[compSet->sparse_Indecies[entity]];
		else
			return nullptr;
	}

	template<typename T>
	T* GetCompOfTArray(unsigned int index) {

		CompSparseSet<T>* compSet = GetCompSparseSet<T>();

		return &compSet->denseTArray[index];
	}

private:
	RandomNumberUtils randomNumber;
};