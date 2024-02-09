#include "../HelpHeaders/SceneManager.h"

void SceneManager::CreateCompSets() {

	transSet = new CompSparseSet<Transform>();
	shaderSet = new CompSparseSet<Shader>();
	meshDataSet = new CompSparseSet<MeshData>();

	camSet = new CompSparseSet<CameraComponent>();

	parentChild = new CompSparseSet<ParentChild>();
	partSet = new CompSparseSet<Particle>();
	partiGroupSet = new CompSparseSet<ParticleGroup>();

	ptLSet = new CompSparseSet<PointLight>();
	sptlSet = new CompSparseSet<SpotLight>();
	drlSet = new CompSparseSet<DirectionalLight>();

	CreateAppCompSets();

}

void SceneManager::PrintSystemNames() {

	SystemsManager::Instance()->engineExitSystem = &engineExit;

	for (int i = 0; i < SystemsManager::Instance()->systems.size(); i++)
	{
		std::cout << "ADDED SYSTEMS: " << SystemsManager::Instance()->systems[i]->systemName << std::endl;
	}

	PrintAppSystemNames();
}