#include "TestSystem.h"

void TestComp::SerializeComponent() {}
void TestComp::DeSerializeComponent() {}
void TestComp::UIfyComponent() {

	UIfyFloat(tstFloat);
	//UIfyBool(a);
	UIfyInt(x1, x2, x3);

}


TestSystem::TestSystem() {

	systemName = typeid(TestSystem).name();
	archetype.priorityType = HAS_ALL_COMP;
	//archetype.compSparseIdsNdPriority.push_back(TestComp::compSetIndex);
}

void TestSystem::PreAwake() {
}

void TestSystem::Awake() {

}

void TestSystem::Start() {

	std::cout << "Wow!" << std::endl;
	//std::cout << "Wow! HOT RELOADED? " << std::endl;

}

void TestSystem::Update() {

	CompSparseSet<TestComp>* curTC = Scene::curScene->GetCompSparseSet<TestComp>();

	for (int i = 0; i < curTC->aprntDenseSize; i++)
	{
		//std::cout << curTC->denseTArray[i].tstFloat << " : " << curTC->denseTArray[i].x1 << curTC->denseTArray[i].x2 << curTC->denseTArray[i].x3 <<  std::endl;
	}
	//std::cout << "Wow! HOT RELOADED? " << std::endl;

}

void TestSystem::LateUpdate() {

}

void TestSystem::Clear() {

}