#pragma once

// FallenOrder SDK

#include <Windows.h>
#include <Psapi.h>
//#include "../SDKOffsetRef.h"
#include "FO_Basic.hpp"
#include "FO_CoreUObject_classes.hpp"
namespace SDK
{

	static void InitSDK()
	{
		
		DWORD64   Names_offset = NULL;
		//int GNameOffset = 0x371DC98;
		Names_offset = (*(DWORD64*)((DWORD64)GetModuleHandleW(0) + 0x371DC98));
		FName::GNames = (TNameEntryArray*)Names_offset;
		
		DWORD64   GObjObjects_offset = NULL;
		//int GObjOffset = 0x3875CD0;
		GObjObjects_offset = (DWORD64)((DWORD64)GetModuleHandleW(0) + 0x3875CD0);
		UObject::GObjects = (FUObjectArray*)GObjObjects_offset;
		
		DWORD64   GWorldObjects = NULL;
		//int GWorldOffset = 0x396BAB8;
		GWorldObjects = (DWORD64)((DWORD64)GetModuleHandleW(0) + 0x396BAB8);
		UWorld::GWorld = (UWorld**)GWorldObjects;
	}
}