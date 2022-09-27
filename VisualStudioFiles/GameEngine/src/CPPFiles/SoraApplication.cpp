#include "../HelpHeaders/SoraApplication.h"

SoraApplication::SoraApplication()
{

}

SoraApplication::~SoraApplication()
{

}

void SoraApplication::ApplicationLoop() {

	//std::filesystem::file_time_type time = std::filesystem::last_write_time(std::filesystem::path(checkFile));
	//std::atomic<std::filesystem::file_time_type> curTime = time;
	//std::cout << "THE STARTED APP!!!" << std::endl;

	SystemsManager::Instance()->RunUnorderedInit();

	SystemsManager::Instance()->RunSystemsPreAwake();
	//std::cout << "PRE-AWAKE OVER" << std::endl;

	SystemsManager::Instance()->RunSystemsAwake();
	//std::cout << "AWAKE OVER" << std::endl;

	SystemsManager::Instance()->RunSystemsStart();
	//std::cout << "Start OVER" << std::endl;

	bool showDemoWindow = true;

	//std::cout << "OVER WITH SETUP!!!" << std::endl;

	bool delayFFrame = true;
	while (!glfwWindowShouldClose(WindowProperties::instance->window)) {

		Time::curTime = (float)glfwGetTime();
		Time::deltaTime = Time::curTime - Time::timeLastFrame;

		Input::Get().SetMouseVals();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!delayFFrame) {
			SystemsManager::Instance()->RunSystemsUpdate();
			SystemsManager::Instance()->RunSystemsLateUpdate();
		}

		delayFFrame = false;

		glfwSwapBuffers(WindowProperties::instance->window);
		glfwPollEvents();

		Time::timeLastFrame = Time::curTime;
		Input::Get().Reset();

		//if (curTime.load() != time) {
		//	glfwSetWindowShouldClose(WindowProperties::instance->window, true);
		//	reload = true;
		//}
	}

	SystemsManager::Instance()->RunSystemsClear();
	SystemsManager::Instance()->ClearSystemsManager();

	Scene::curScene->sceneTabler.DestComps();

	glfwTerminate();
}

