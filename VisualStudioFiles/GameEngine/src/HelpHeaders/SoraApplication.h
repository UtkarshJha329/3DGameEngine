#pragma once

#include <iostream>
#include <string>

#include "../HelpHeaders/TimeVals.h"
#include "../HelpHeaders/Components/Input.h"
#include "../HelpHeaders/WindowProperties.h"
#include "../HelpHeaders/EditorGuiVars.h"

#include "../HelpHeaders/EntityTabler.h"
#include "../HelpHeaders/SystemsManager.h"

#include "../HelpHeaders/SystemsOrderManager.h"
#include "../HelpHeaders/SceneManager.h"

#include <Windows.h>
#include <filesystem>
#include <tchar.h>
#include <future>

class SoraApplication {

public:

	SoraApplication();

	virtual ~SoraApplication();

	void ApplicationLoop();

	inline static LPCWSTR checkFile = _T("D:\\GitKrakenRepos\\3DGameEngineProject\\VisualStudioFiles\\Sandbox\\src\\TestSystem.h");
};