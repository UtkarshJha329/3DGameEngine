#include "../HelpHeaders/SystemsManager.h"

 //std::vector<System*> SystemsManager::systems;
 //unsigned int SystemsManager::systemBeginInt;

 //std::vector<unsigned int> SystemsManager::systemsTraversOrderIndex;

 //unsigned int SystemsManager::curSmallestSortedSetSize = 0;

void SystemsManager::RunUnorderedInit()
{
	for (int i = 0; i < systems.size(); i++)
	{
		systems[i]->UnorderedInit();
		//std::cout << systems[i]->systemName << " : " << systems[i]->systemID << std::endl;
	}
}

void SystemsManager::RunSystemsPreAwake() {

	 //MakeTraversalOrder();

	 for (unsigned int i = 0; i < systemsTraversOrderIndex.size(); i++)
	 {
		 System* curSystem = systems[systemsTraversOrderIndex[i]];

		 SortCompSet(curSystem->archetype);

		 curSystem->curSmallestSetSize = curSmallestSortedSetSize;
		 curSystem->PreAwake();
	 }
 }

 void SystemsManager::RunSystemsAwake() {
	 
	 //MakeTraversalOrder();

	 for (unsigned int i = 0; i < systemsTraversOrderIndex.size(); i++)
	 {

		 System* curSystem = systems[systemsTraversOrderIndex[i]];

		 //std::cout << i << std::endl;
		 //if (curSystem->archetype.compSparseIdsNdPriority.size() > 0) {
		 //
		//	 unsigned int compId = curSystem->archetype.compSparseIdsNdPriority[0];
		//	 std::cout << curSystem->curScene->sceneTabler.allCmpsSSets[compId]->compName << std::endl;
		 //}

		 SortCompSet(curSystem->archetype);

		 curSystem->curSmallestSetSize = curSmallestSortedSetSize;
		 curSystem->Awake();
	 }
 }

 void SystemsManager::RunSystemsStart()
 {
	 //MakeTraversalOrder();

	 for (unsigned int i = 0; i < systemsTraversOrderIndex.size(); i++)
	 {
		 System* curSystem = systems[systemsTraversOrderIndex[i]];

		 SortCompSet(curSystem->archetype);

		 curSystem->curSmallestSetSize = curSmallestSortedSetSize;
		 curSystem->Start();
	 }

 }

 void SystemsManager::RunSystemsUpdate()
 {
	 //MakeTraversalOrder();

	 for (unsigned int i = 0; i < systemsTraversOrderIndex.size(); i++)
	 {
		 System* curSystem = systems[systemsTraversOrderIndex[i]];

		 SortCompSet(curSystem->archetype);

		 curSystem->curSmallestSetSize = curSmallestSortedSetSize;
		 curSystem->Update();
	 }

 }

 void SystemsManager::RunSystemsLateUpdate()
 {
	 //MakeTraversalOrder();

	 for (unsigned int i = 0; i < systemsTraversOrderIndex.size(); i++)
	 {
		 System* curSystem = systems[systemsTraversOrderIndex[i]];

		 SortCompSet(curSystem->archetype);

		 curSystem->curSmallestSetSize = curSmallestSortedSetSize;
		 curSystem->LateUpdate();
	 }
 }

 void SystemsManager::RunSystemsClear()
 {
	 //MakeTraversalOrder();
	 for (unsigned int i = 0; i < systemsTraversOrderIndex.size(); i++)
	 {
		 System* curSystem = systems[systemsTraversOrderIndex[i]];

		 SortCompSet(curSystem->archetype);

		 curSystem->curSmallestSetSize = curSmallestSortedSetSize;
		 curSystem->Clear();
		 
	 }

 }

 void SystemsManager::MakeTraversalOrder() {

	 std::queue<unsigned int> traversalQueue;

	 traversalQueue.push(systemBeginInt);
	 systemsTraversOrderIndex.clear();
	 systemsTraversOrderIndex.emplace_back(systemBeginInt);

	 //ENGINE SYSTEMS
	 while (!traversalQueue.empty()) {

		 unsigned int curSystemIndex = traversalQueue.front();
		 System* curSystem = systems[curSystemIndex];

		 for (unsigned int i = 0; i < curSystem->nxtSystemsIndecies.size(); i++)
		 {
			 unsigned int nxtSystemIndex = curSystem->nxtSystemsIndecies[i];
			 systemsTraversOrderIndex.emplace_back(nxtSystemIndex);
			 traversalQueue.push(nxtSystemIndex);
		 }

		 traversalQueue.pop();
	 }

	 //std::cout << engineExitSystem->systemID << std::endl;

	 systemsTraversOrderIndex.emplace_back(engineExitSystem->systemID);
	 traversalQueue.push(engineExitSystem->systemID);

	 //APPLICATION SYSTEMS
	 while (!traversalQueue.empty()) {

		 unsigned int curSystemIndex = traversalQueue.front();
		 System* curSystem = systems[curSystemIndex];

		 for (unsigned int i = 0; i < curSystem->nxtSystemsIndecies.size(); i++)
		 {
			 unsigned int nxtSystemIndex = curSystem->nxtSystemsIndecies[i];
			 systemsTraversOrderIndex.emplace_back(nxtSystemIndex);
			 traversalQueue.push(nxtSystemIndex);
		 }

		 traversalQueue.pop();
	 }

	 std::queue<unsigned int>().swap(traversalQueue);
 }

 void SystemsManager::SortCompSet(Archetype arc) {

	 curSmallestSortedSetSize = 0;

	 std::vector<BaseSparseSet*> setsToSort;

	 bool first = true;
	 std::pair<unsigned int, unsigned int> sizeOfSmalNdIndex = std::pair<unsigned int, int>(0, 0);

	 Scene* curScene = Scene::curScene;

	 for (unsigned int compIndex : arc.compSparseIdsNdPriority)
	 {
		 setsToSort.push_back(curScene->sceneTabler.allCmpsSSets[compIndex]);
		 unsigned int curCompSetIndex = setsToSort.size() - 1;
		 unsigned int sizeofDense = setsToSort[curCompSetIndex]->DenseSize();

		//std::cout << "COMP INDEX : " << compIndex
		//			<< "\tDENSE SIZE: " << sizeofDense
		//			<< "\tOld dense size: " << sizeOfSmalNdIndex.first
		//			<<	"IS less: " << (sizeofDense < sizeOfSmalNdIndex.first)
		//			<< std::endl;


		 if (first || sizeofDense < sizeOfSmalNdIndex.first) {

			 sizeOfSmalNdIndex.first = sizeofDense;
			 sizeOfSmalNdIndex.second = curCompSetIndex;
			 first = false;
		 }
	 }

	 //std::cout << sizeOfSmalNdIndex.second << std::endl;

	 if (arc.priorityType == HAS_ALL_COMP) {

		 unsigned int curEntityID;

		 unsigned int smallestIndex = sizeOfSmalNdIndex.second;

		 bool hasAllComp = false;
		 unsigned int swapCount = 0;
		 unsigned int ittSize = sizeOfSmalNdIndex.first - swapCount;

		 //Loop through all the entities with components
		 for (unsigned int i = 0; i < ittSize; i++)
		 {
			 hasAllComp = true;
			 curEntityID = setsToSort[smallestIndex]->denseIndexArray[i];

			 //Check if the entity has all the components, 
			 //IF IT DOES NOT: then only swap the current entity index in the smallest component with the last index
			 //in the denseArray
			 for (unsigned int j = 0; j < setsToSort.size(); j++)
			 {
				 if (!setsToSort[j]->EntityHasComp(curEntityID)) {
					 hasAllComp = false;
					 setsToSort[smallestIndex]->SwapDenseEntities(curEntityID
						 , setsToSort[smallestIndex]->denseIndexArray[ittSize - 1]);

					 swapCount++;
					 ittSize = sizeOfSmalNdIndex.first - swapCount;
					 i--;
					 break;
				 }
			 }

			 //IF ALL THE COMPONENTS are in the current Entity then in all the components bring this entity to the front.
			 if (hasAllComp) {
				 for (unsigned int j = 0; j < setsToSort.size(); j++)
				 {
					 setsToSort[j]->SwapDenseEntities(curEntityID, setsToSort[j]->denseIndexArray[i]);
				 }
			 }
		 }

		 curSmallestSortedSetSize = ittSize;

	 }

 }