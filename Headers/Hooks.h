#pragma once
#include "GlobalHeader.h"
#include "Globals.h"
#include "Logger.h"
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include "FObjectSpawnParameters.h"
#include "dllmain.h"
#include <algorithm>
#include "MinHook.h"
namespace fs = std::experimental::filesystem;
bool FirstLoad = true;
namespace Hooks
{
	namespace HookedFunctions
	{
		PVOID(*origInitGameState)(void*);
		PVOID hookInitGameState(void* Ret)
		{
			Log::Info("GameStateHook");
			if (FirstLoad)
			{
				SDK::InitSDK();
				Log::Info("Engine Classes Loaded");
				FirstLoad = false;
			}
			for (int i = 0; i < Global::ModActors.size(); i++)
			{
				SDK::AActor* CurrentModActor = Global::ModActors[i];
				if (CurrentModActor != nullptr)
				{
					Functions::FOutputDevice ar;
					Functions::CallFunctionByNameWithArguments(CurrentModActor, L"ModCleanUp", &ar, NULL, true);
				}
			}
			Global::ModActors.clear();
			Functions::FOutputDevice ar;
			Functions::CallFunctionByNameWithArguments(Global::ModLoaderActor, L"CleanLoader", &ar, NULL, true);
			Global::ModLoaderActor = nullptr;
			if (Functions::addr_StaticLoadObject)
			{
				SDK::FTransform transform;
				transform.Translation = Functions::makeVector(0, 0, 0);
				transform.Rotation = Functions::makeQuat(0, 0, 0, 0);
				transform.Scale3D = Functions::makeVector(1, 1, 1);
				FActorSpawnParameters* spawnParams = &FActorSpawnParameters::FActorSpawnParameters();
				SDK::UClass* ModActorObject = Functions::LoadClassFromString(L"/Game/ModLoaderContent/ModLoaderActor.ModLoaderActor_C", false);
				if (ModActorObject)
				{
					Log::Info("Sucessfully Loaded ModLoader Pak");
				}
				Global::ModLoaderActor = Functions::SpawnActor(SDK::UWorld::GetWorld(), ModActorObject, &transform, spawnParams);
				for (int i = 0; i < Global::modnames.size(); i++)
				{
					std::wstring CurrentMod;
					//StartSpawningMods
					CurrentMod = Global::modnames[i];
					std::string str(CurrentMod.begin(), CurrentMod.end());
					const std::wstring Path = L"/Game/Mods/" + CurrentMod + L"/ModActor.ModActor_C";
					SDK::UClass* ModObject = Functions::LoadClassFromString(Path.c_str(), false);
					if (ModObject)
					{
						SDK::AActor* ModActor;
						
						ModActor = Functions::SpawnActor(SDK::UWorld::GetWorld(), ModObject, &transform, spawnParams);
						if (ModActor)
						{
							Global::ModActors.push_back(ModActor);
							Functions::FOutputDevice ar;
							Functions::CallFunctionByNameWithArguments(ModActor, L"PreBeginPlay", &ar, NULL, true);
							Log::Info("Sucessfully Loaded %s", str);
						}
					}
					else
					{
						Log::Info("Could not locate ModActor for %s", str);
					}
				}
					Log::Info("Finished Spawning PakMods");
				}
			Log::Info("Returning to GameState --------------------------------------------------------");
			return origInitGameState(Ret);
		}
		PVOID(*origBeginPlay)(SDK::AActor*);
		PVOID hookBeginPlay(SDK::AActor* Actor)
		{
			if (Actor->Class == SDK::ABP_Hero_C::StaticClass())
			{
				Functions::FOutputDevice ar;
				Functions::CallFunctionByNameWithArguments(Global::ModLoaderActor, L"PostLoaderStart", &ar, NULL, true);
				for (int i = 0; i < Global::ModActors.size(); i++)
				{
					SDK::AActor* CurrentModActor = Global::ModActors[i];
					if (CurrentModActor != nullptr)
					{
						Functions::CallFunctionByNameWithArguments(CurrentModActor, L"PostBeginPlay", &ar, NULL, true);
					}
				}
			}
			return origBeginPlay(Actor);
		}

		PVOID(*origSay)(void*, SDK::FString *);
		PVOID hookSay(void* GM, SDK::FString* Message)
		{
			std::wstring WStrMsg = Message->c_str();
			if (WStrMsg.substr(WStrMsg.length() - 6, 6) == L"/Print") // check if message ends with /Print
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				WStrMsg = WStrMsg.substr(0, WStrMsg.length() - 6); // remove /Print extension
				std::string str(WStrMsg.begin(), WStrMsg.end());
				SetConsoleTextAttribute(hConsole, 13);
				std::cout << "Print: " << str << std::endl;
				SetConsoleTextAttribute(hConsole, 7);
			}
			return origSay(GM, Message);
		}
	}


	DWORD __stdcall InitHooks(LPVOID)
	{
		Hook::Init();
		DWORD_PTR GameStateInit_addy = (DWORD_PTR)Pattern::Find("40 53 48 83 EC 20 48 8B 41 10 48 8B D9 48 8B 91 C0 03 00 00");
		DWORD_PTR Begin_addy = (DWORD_PTR)Pattern::Find("49 89 E3 48 81 EC 28 01 00 00 48 8B 05 F7 A0 03 F9 48 31 E0");
		DWORD_PTR Say_addy = (DWORD_PTR)Pattern::Find("33 C0 4C 8B C2 48 89 44 24 08 33 D2");
		if (GameStateInit_addy)
		{
			ScanLoadedPaks();
		}
		Hook::Add(GameStateInit_addy, &HookedFunctions::hookInitGameState, &HookedFunctions::origInitGameState, "AGameModeBase::InitGameState");
		Hook::Add(Begin_addy, &HookedFunctions::hookBeginPlay, &HookedFunctions::origBeginPlay, "AActor::BeginPlay");
		Hook::Add(Say_addy, &HookedFunctions::hookSay, &HookedFunctions::origSay, "AGameMode::Say");

		
		return NULL;
	}
};