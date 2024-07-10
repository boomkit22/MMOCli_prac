#pragma once

//UENUM(BlueprintType)
//enum class ECharacterType : uint8
//{
//	ECT_Warrior,
//	ECT_Mage,
//	ECT_Rogue
//};

UENUM(BlueprintType)
enum class ECharacterClassType : uint8
{
	CCT_None UMETA(DisplayName = "None"),
	CCT_Sword UMETA(DisplayName = "Sword"),
	CCT_Axe UMETA(DisplayName = "Axe"),
};

enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
};

#pragma once
UENUM(BlueprintType)
enum class EMovingState : uint8
{
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_Move UMETA(DisplayName = "Move"),
	EMS_Idle UMETA(DisplayName = "Idle"),
};

enum class ECharacterSkill : uint8
{
	ECS_Combo1,
	ECS_Combo1_1,
};