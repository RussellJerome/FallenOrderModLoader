#pragma once
#include "SDK/SDK.hpp"
#include <map>
#include <iostream>
#include <string>

class Global {
public:
	static std::vector<std::wstring> modnames;
	static std::vector<SDK::AActor*> ModActors;
	static std::string GameName;
};
