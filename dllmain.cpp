#include "GlobalHeader.h"
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include "dllmain.h"
#include "INI/INI.h"

namespace fs = std::filesystem;
HANDLE                      m_hConsole;
WORD                        m_currentConsoleAttr;
CONSOLE_SCREEN_BUFFER_INFO   csbi;

void ScanLoadedPaks()
{
	char path_c[MAX_PATH];
	GetModuleFileNameA(NULL, path_c, MAX_PATH);
	std::string path = std::string(path_c);
	path = path.substr(0, path.find_last_of("/\\"));
	path = path.substr(0, path.find_last_of("/\\"));
	path = path.substr(0, path.find_last_of("/\\"));
	path = path + "\\Content\\Paks\\LogicMods\\";
	Global::modnames.empty();
	for (const auto & entry : fs::directory_iterator(path))
	{
		if (entry.path().extension().string() == ".pak")
		{
			std::wstring filename = entry.path().filename().wstring();
			std::wstring modNameW = filename.substr(0, filename.length() - 4);
			if (modNameW.substr(modNameW.length() - 2, 2) == L"_p")
				modNameW = modNameW.substr(0, modNameW.length() - 2);
			if (modNameW.substr(modNameW.length() - 2, 2) == L"_P")
				modNameW = modNameW.substr(0, modNameW.length() - 2);
			std::string str(modNameW.begin(), modNameW.end());
			Log::Info("PakModLoaded: %s", str);
			Global::modnames.push_back(modNameW);
		}
	}
}

void StartUpLoader()
{
	Log::Info("Fallen Order ModLoader V1.1.1 ~Created by Russell.J");
	Log::Info("Injected successfully...");
	INI::PARSE_FLAGS = INI::PARSE_COMMENTS_ALL | INI::PARSE_COMMENTS_SLASH | INI::PARSE_COMMENTS_HASH;
	INI ini("ModLoaderInfo.ini", true);
	//Output File Initialization
	ini.select("DEBUG");
	if (ini.getAs<int>("DEBUG", "UseConsole", 0) == 1)
	{
		ShowWindow(GetConsoleWindow(), SW_SHOW);
		FreeConsole();
		AllocConsole();
		freopen("CON", "w", LOG_STREAM);
	}
	CreateThread(0, 0, Functions::InitFunctionCalls, 0, 0, 0);
	CreateThread(0, 0, Hooks::InitHooks, 0, 0, 0);
}