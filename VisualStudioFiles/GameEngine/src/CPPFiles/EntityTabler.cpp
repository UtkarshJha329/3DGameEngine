#include "../HelpHeaders/EntityTabler.h"

#pragma region ENTITY_SPARSE_SET

unsigned int EntitySparseSet::NewEntity(bool _serializable) {

	unsigned int entity;

	if (available > 1) {

		//std::cout << "AVAILABLE USED" << std::endl;

		entity = freeHead;

		//if (freeHead != sparse_Index[entity]) {
		//	freeHead = sparse_Index[entity];
		//}

		freeHead = sparse_Index[entity];

		sparse_Index[entity] = filledDenseEntities;
		entity_Alive[entity] = true;
		entity_serializable[entity] = _serializable;


		dense_Entities[filledDenseEntities] = entity;
		filledDenseEntities++;
		available--;

		return entity;
	}

	//std::cout << "NEW CREATED" << std::endl;

	entity = sparse_Index.size();
	sparse_Index.emplace_back(filledDenseEntities);
	dense_Entities.emplace_back(entity);
	filledDenseEntities++;
	entity_Alive.emplace_back(true);
	entity_serializable.emplace_back(_serializable);

	//std::cout << "Entity ID: " << entity << std::endl;

	return entity;

}

void EntitySparseSet::DeleteEntity(unsigned int entity) {

	unsigned int curDenseIndex = sparse_Index[entity];
	unsigned int lastDenseVal = dense_Entities[filledDenseEntities - 1];

	dense_Entities[curDenseIndex] = lastDenseVal;

	sparse_Index[lastDenseVal] = curDenseIndex;

	//BRANCHLESS
	//sparse_Index[entity] = freeHead * (available > 0);

	if (available > 0) {
		sparse_Index[entity] = freeHead;
	}
	
	freeHead = entity;
	available++;

	filledDenseEntities--;
	//dense_Entities.pop_back();
	entity_Alive[entity] = false;
	entity_serializable[entity] = false;
}

#pragma endregion

CompChangeDetector* CompChangeDetector::inst;

#pragma region TABLER

void Tabler::RmvEntity(unsigned int entity)
{
	for (unsigned int i = 0; i < allCmpsSSets.size(); i++)
	{
		allCmpsSSets[i]->Remove(entity);
	}

	entity_Set.DeleteEntity(entity);
}
#pragma endregion
