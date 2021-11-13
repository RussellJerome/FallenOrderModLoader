#pragma once
#include <Windows.h>
#include "GlobalHeader.h"
#include "SDK/SDK.hpp"
#include "FObjectSpawnParameters.h"
#include "Globals.h"
namespace Functions
{
	class FOutputDevice {
	protected:
		bool bSuppressEventTag;
		bool bAutoEmitLineTerminator;
	};

	DWORD_PTR addr_StaticLoadObject;
	SDK::UObject* StaticLoadObject(SDK::UClass *uclass, SDK::UObject * InOuter, const wchar_t *InName, const wchar_t *Filename, unsigned int LoadFlags, SDK::UPackageMap * Sandbox, bool bAllowObjectReconciliation)
	{
		return reinterpret_cast<SDK::UClass*(__fastcall*)(SDK::UClass *, SDK::UObject *, const wchar_t *, const wchar_t *, unsigned int, SDK::UPackageMap *, bool)>(addr_StaticLoadObject)(uclass, InOuter, InName, Filename, LoadFlags, Sandbox, bAllowObjectReconciliation);
	}

	SDK::UClass* LoadClassFromString(const wchar_t *InName, bool bAllowObjectReconciliation)
	{
		return (SDK::UClass*)StaticLoadObject(SDK::UClass::StaticClass(), nullptr, InName, nullptr, 0, nullptr, bAllowObjectReconciliation);
	}

	DWORD_PTR addr_CallFunctionByNameWithArguments;
	// Calls a function by name, requires the Object you want to call from.
	SDK::UObject* CallFunctionByNameWithArguments(SDK::UObject* Object, const wchar_t *Str, FOutputDevice *Ar, SDK::UObject *Executor, bool bForceCallWithNonExec)
	{
		return reinterpret_cast<SDK::UObject*(__fastcall*)(SDK::UObject*, const wchar_t *, FOutputDevice *, SDK::UObject *, bool)>(addr_CallFunctionByNameWithArguments)(Object, Str, Ar, Executor, bForceCallWithNonExec);
	}

	DWORD_PTR addr_SpawnActor;
	SDK::AActor* SpawnActor(SDK::UWorld* uworld, SDK::UClass* uclass, const  SDK::FTransform* transform, const FActorSpawnParameters* params)
	{
		return reinterpret_cast<SDK::AActor*(__fastcall*)(SDK::UWorld*, SDK::UClass*, const SDK::FTransform*, const FActorSpawnParameters*)>(addr_SpawnActor)(uworld, uclass, transform, params);
	}

	SDK::FVector makeVector(float x, float y, float z) {
		SDK::FVector vec = SDK::FVector();
		vec.X = x;
		vec.Y = y;
		vec.Z = z;
		return vec;
	}
	SDK::FRotator makeRotator(float pitch, float roll, float yaw) {
		SDK::FRotator rot = SDK::FRotator();
		rot.Pitch = pitch;
		rot.Roll = roll;
		rot.Yaw = yaw;
		return rot;
	}

	SDK::FQuat makeQuat(float W, float X, float Y, float Z) {
		SDK::FQuat quat = SDK::FQuat();
		quat.W = W;
		quat.X = X;
		quat.Y = Y;
		quat.Z = Z;
		return quat;
	}

	SDK::FString MakeFString(std::string String)
	{
		return SDK::FString(std::wstring(String.begin(), String.end()).c_str());
	}

	DWORD __stdcall InitFunctionCalls(LPVOID)
	{
		
		addr_StaticLoadObject = (DWORD_PTR)Pattern::Find("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 44 8B BD ? ? ? ? 48 8D 44 24");

		addr_SpawnActor = (DWORD_PTR)Pattern::Find("48 8B C4 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 78 98 44 0F 29 40");

		addr_CallFunctionByNameWithArguments = (DWORD_PTR)Pattern::Find("40 55 53 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 4C 89 85 ? ? ? ? 48 8B F1 48 89 4D 38");
		return NULL;
	}
};