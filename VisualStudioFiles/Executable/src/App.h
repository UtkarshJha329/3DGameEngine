#pragma once

#include <iostream>
#include <Windows.h>

#include <filesystem>

#include <future>

#include <tchar.h>

#include <any>

typedef void (*ApplicationSys)();

void LoadScriptsDLL(HINSTANCE* libInst, ApplicationSys* appSys = new ApplicationSys());

//LPCWSTR loadFile = _T("D:\\GitKrakenRepos\\3DGameEngineProject\\VisualStudioFiles\\OutputOfAll\\Sandbox.dll");
LPCWSTR loadFile = _T("Sandbox.dll");

int main() {

	HINSTANCE* libInst = new HINSTANCE();
	ApplicationSys* appSys = new ApplicationSys();

	std::cout << "LOADING" << std::endl;

	LoadScriptsDLL(libInst, appSys);

	(*appSys)();

	FreeLibrary(*libInst);

	delete appSys;
	delete libInst;

	char c = 'n';
	std::cin >> c;

	return 0;
}

void LoadScriptsDLL(HINSTANCE* libInst, ApplicationSys* appSys) {

	if (*libInst) {
		FreeLibrary(*libInst);
	}

 	*libInst = LoadLibrary(loadFile);

	if (!(*libInst)) {
		std::cout << GetLastError() << std::endl;
		std::cout << "Couldn't load dll." << std::endl;
		return;
	}

	*appSys = (ApplicationSys)GetProcAddress(*libInst, "ApplicationSystems");

	if (!(*appSys)) {
		std::cout << "COULDN'T LOAD METHOD." << std::endl;
		return;
	}

}
