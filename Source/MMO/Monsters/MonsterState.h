#pragma once

UENUM(BlueprintType)
enum class EMonsterType : uint16
{
	EMT_None UMETA(DisplayName = "None"),
	EMT_Guardian UMETA(DisplayName = "Sword"),
	EMT_Spider UMETA(DisplayName = "Axe"),
};

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_Move UMETA(DisplayName = "Move"),
	EMS_Idle UMETA(DisplayName = "Idle"),
};