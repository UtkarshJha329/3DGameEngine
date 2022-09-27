#pragma once

#include "TestSystem.h"

#include "Systems/Header/CameraMovementSystem.h"

#include "CompDestroySystem.h"

#include "SoraApplication.h"

#include <any>


class AppSceneManager : public SceneManager {

public:

	AppSceneManager() {
		std::cout << "STARTED SCENE MANAGER." << std::endl;
	}

	~AppSceneManager() {
		DestroyCompSets();
	}

	virtual void CreateAppCompSets() override {
		tstCompSet = new CompSparseSet<TestComp>();
		camMvSet = new CompSparseSet<CameraMovementComponent>();
	}

	virtual void PrintAppSystemNames() override {
		std::cout << tstSystem.systemName << std::endl;
		std::cout << camMvSys.systemName << std::endl;
		std::cout << compDesSystem.systemName << std::endl;
	}

	void DestroyCompSets() {
		tstCompSet->~CompSparseSet();
		camMvSet->~CompSparseSet();
	}

private:

	//Depending on control these should be unique or shared ptrs...
	CompSparseSet<TestComp>* tstCompSet;
	CompSparseSet<CameraMovementComponent>* camMvSet;

	TestSystem tstSystem;
	CameraMovementSystem camMvSys;
	CompDestroySystem compDesSystem;

};

class Sandbox : public SoraApplication {

public:

	Sandbox() {
	}

	~Sandbox() {

	}
};

//int main(int argc, char** argv) {
//
//	ApplicationSystems();
//
//}

using namespace std::chrono_literals;

extern "C" {
	__declspec(dllexport) void ApplicationSystems() {

		std::cout << "STARTING APP..." << std::endl;

		std::shared_ptr<CompChangeDetector> ccd = nullptr;
		std::shared_ptr<AppSceneManager> scm = nullptr;
		std::shared_ptr<SystemsManager> sysMang = nullptr;
		std::shared_ptr<Sandbox> application = nullptr;
		std::shared_ptr<WindowProperties> windProps = nullptr;
		std::shared_ptr<CommonMeshData> md = nullptr;
		
		EditorGuiVars* editorGuiVars = new EditorGuiVars();

		std::cout << "NEW CompChangeDetector CREATED." << std::endl;

		ccd = std::make_shared<CompChangeDetector>();
		ccd->SetInstance();

		std::cout << "NEW SystemsManager CREATED." << std::endl;

		sysMang = std::make_shared <SystemsManager>();
		sysMang->SetInstance();

		std::cout << "NEW AppSceneManager CREATED." << std::endl;

		scm = std::make_shared <AppSceneManager>();
		scm->CreateCompSets();
		scm->PrintSystemNames();

		std::cout << "NEW AppOrderManager CREATED." << std::endl;

		application = std::make_shared <Sandbox>();

		windProps = std::make_shared<WindowProperties>();
		windProps.get()->instance = windProps.get();

		md = std::make_shared<CommonMeshData>();
		md.get()->SetInstance();

		application->ApplicationLoop();

	}
}