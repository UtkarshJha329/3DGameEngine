#include "../../HelpHeaders/Systems/SerializationSystem.h"

void SerializationSystem::UnorderedInit()
{
	std::cout << "Setting up new system Order." << std::endl;

	RandomProps::curInFileStream.open("SystemsTraversalOrder.txt");

	std::string curLine = "";
	while (std::getline(RandomProps::curInFileStream, curLine)) {
		//std::cout << "READ TRAVERSAL ORDER File." << std::endl;
		int curSysId = 0;
		std::stringstream sysIds(curLine);
		sysIds >> curSysId;

		std::cout << curSysId << std::endl;

		SystemsManager::Instance()->systemsTraversOrderIndex.push_back(curSysId);
	}

	RandomProps::curInFileStream.close();

}

void SerializationSystem::PreAwake() {

}

void SerializationSystem::Awake() {

	//IN CASE OF HOT RELOADING
	Tabler* curTab = &Scene::curScene->sceneTabler;
	//CreateTabler(curTab);
	//std::cout << "RELOADING SAVE." << std::endl;
	LoadToTabler(curTab, false);


}

void SerializationSystem::Start() {

}

void SerializationSystem::Update() {

	Tabler* curTab = &Scene::curScene->sceneTabler;

	//if (RandomProps::reloadOnce) {
	//	//std::cout << "RELOADING SAVE." << std::endl;
	//	//LoadToTabler(curTab, true);

	//	RandomProps::reloadOnce = false;
	//}

#pragma region Saving

	if (Input::Get().KeyDown(KeyCode::S) && Input::Get().KeyHeld(KeyCode::LEFT_CTRL)) {

		Tabler* curSceneTabler = &(curScene->sceneTabler);
		unsigned int numEntitiesAlive = curSceneTabler->entity_Set.filledDenseEntities;

		unsigned int numEntitiesSerializable = 0;
		for (unsigned int i = 0; i < curSceneTabler->entity_Set.sparse_Index.size(); i++)
		{
			if (curSceneTabler->entity_Set.entity_serializable[i]) {
				numEntitiesSerializable++;
			}
		}

		RandomProps::curSaveStr += "Number of Entities Alive : "
			+ std::to_string(numEntitiesSerializable)
			+ "\n\n";

		for (unsigned int i = 0; i < curTab->allCmpsSSets.size(); i++)
		{
			std::string sizeOfComp = std::to_string(curTab->allCmpsSSets[i]->DenseSize());
			//RandomProps::curSaveStr += curTab->allCmpsSSets[i]->compName + " : " + sizeOfComp + "\n\n";
			unsigned int sizeOfCompSet = curSceneTabler->allCmpsSSets[i]->DenseSize();
			RandomProps::curSaveStr += curSceneTabler->allCmpsSSets[i]->compName + " : ";

			unsigned int numSerializableComps = 0;
			std::string continuation = "";
			for (unsigned int j = 0; j < numEntitiesAlive; j++)
			{
				bool xo = true;
				if (curSceneTabler->allCmpsSSets[i]->EntityHasComp(j)) {
					//RandomProps::curSaveStr += std::to_string(j);
					if (curSceneTabler->allCmpsSSets[i]->Serializable(j)) {
						continuation += "o";
						numSerializableComps++;
					}
					else {
						xo = false;
					}
				}
				else if(curSceneTabler->entity_Set.entity_serializable[j] && xo){
					continuation += "x";
				}
			}

			RandomProps::curSaveStr += std::to_string(numSerializableComps) + " : " + continuation;

			RandomProps::curSaveStr += "\n\n";


			curTab->allCmpsSSets[i]->SerializeCompSet();

			RandomProps::curSaveStr += "\n";
			RandomProps::curSaveStr += "---------------------------------------------------------------------";
			RandomProps::curSaveStr += "\n";
		}

		DumpCurSaveStrToFile();

		SystemsManager* inst = SystemsManager::Instance();
		for (int i = 0; i < inst->systemsTraversOrderIndex.size(); i++)
		{
			RandomProps::curSaveStr += std::to_string(inst->systemsTraversOrderIndex[i]) + "\n";
		}
		RandomProps::curOutFileStream.open("SystemsTraversalOrder.txt");
		RandomProps::curOutFileStream << RandomProps::curSaveStr;
		RandomProps::curOutFileStream.close();

		RandomProps::curSaveStr = "";
	}
#pragma endregion

#pragma region Reloading

	if (Input::Get().KeyDown(KeyCode::R) && Input::Get().KeyHeld(KeyCode::LEFT_CTRL)) {

		LoadToTabler(curTab, true);
	}

#pragma endregion

}

void SerializationSystem::LateUpdate() {

}

void SerializationSystem::Clear() {

	Tabler* curTab = &Scene::curScene->sceneTabler;
}

//INEFFIECIENT
void SerializationSystem::LoadToTabler(Tabler* curTab, bool onlyFill) {

	RandomProps::curInFileStream.open(RandomProps::saveFileName);

	std::string skipStr = "";
	std::vector<std::string> curLineSplit;

	std::getline(RandomProps::curInFileStream, skipStr);
	//std::cout << "READ NEW LINE : " << skipStr << std::endl;
	if (!onlyFill) {
		//std::cout << "READ NEW LINE : " << skipStr << std::endl;
		curLineSplit = Split(skipStr, " : ");

		std::stringstream totalNumEntities(curLineSplit[1]);
		int numEntities = 0;
		totalNumEntities >> numEntities;
		//std::cout << "Total Num Entities : " << numEntities << std::endl;

		curTab->entity_Set.Reserve(numEntities);
		for (int i = 0; i < numEntities; i++)
		{
			curTab->entity_Set.NewEntity();
		}
	}
	std::getline(RandomProps::curInFileStream, skipStr);
	//std::cout << "READ NEW LINE : " << skipStr << std::endl;
	int saveSet = -1;
	for (unsigned int i = 0; i < curTab->allCmpsSSets.size(); i++)
	{
		std::getline(RandomProps::curInFileStream, skipStr);
		//std::cout << "READ NEW LINE : " << skipStr << std::endl;

		curLineSplit = Split(skipStr, " : ");
		bool foundComp = false;
		for (int j = 0; j < curTab->allCmpsSSets.size(); j++)
		{
			if (std::strcmp(curTab->allCmpsSSets[j]->compName.data(), curLineSplit[0].data()) == 0) {
				saveSet = j;
				//std::cout << "SAVED SET: " << saveSet << std::endl;
				foundComp = true;
				break;
			}
		}

		if (foundComp) {

			std::stringstream curQuantity(curLineSplit[1]);
			int numComps = 0;
			curQuantity >> numComps;


			if (!onlyFill) {

				if (std::strcmp(curTab->allCmpsSSets[saveSet]->compName.data(), curLineSplit[0].data()) == 0) {

					//std::cout << "EQUAL : " << (saveSet == i) << std::endl;

					int k = 0;
					for (int j = 0; j < curLineSplit[2].length(); j++)
					{
						//std::cout << (curLineSplit[2])[j] << std::endl;
						if ((curLineSplit[2])[j] == 'o') {
							curTab->allCmpsSSets[saveSet]->Add(j);
							//std::cout << "Added : " + curTab->allCmpsSSets[i]->compName + " to : " + std::to_string(j) << std::endl;
							k++;
							if (k >= numComps) {
								break;
							}
						}
					}
				}
			}


			std::getline(RandomProps::curInFileStream, skipStr);
			//std::cout << "DESERIALIZED : " << i << " : " << skipStr << std::endl;
			curTab->allCmpsSSets[saveSet]->DeSerializeCompSet(numComps);

			for (unsigned int j = 0; j < 2; j++)
			{
				std::getline(RandomProps::curInFileStream, skipStr);
				//std::cout << skipStr << std::endl;
			}
		}
	}

	RandomProps::curInFileStream.close();
}

void SerializationSystem::DumpCurSaveStrToFile() {

	RandomProps::curOutFileStream.open(RandomProps::saveFileName);
	RandomProps::curOutFileStream << RandomProps::curSaveStr;
	RandomProps::curOutFileStream.close();


	//std::cout << RandomProps::curSaveStr;
	RandomProps::curSaveStr = "";

}