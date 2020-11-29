// FallenOrder SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

#include "FO_Basic.hpp"
#include "FO_CoreUObject_classes.hpp"

namespace SDK
{
TNameEntryArray* FName::GNames = nullptr;
FUObjectArray* UObject::GObjects = nullptr;
UWorld** UWorld::GWorld = nullptr;

UObject* FWeakObjectPtr::Get() const
{
	auto ObjectItem = UObject::GetGlobalObjects().GetByIndex(ObjectIndex);
	if (ObjectItem.Object != NULL)
	{
		return ObjectItem.Object;
	}
}

bool FWeakObjectPtr::IsValid() const
{
	return true;
}
}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
