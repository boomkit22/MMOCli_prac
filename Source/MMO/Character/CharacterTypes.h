#pragma once

//UENUM(BlueprintType)
//enum class ECharacterType : uint8
//{
//	ECT_Warrior,
//	ECT_Mage,
//	ECT_Rogue
//};

enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
};

#pragma once
UENUM(BlueprintType)
enum class EMovingState : uint8
{
	EMS_Move UMETA(DisplayName = "Move"),
	EMS_Idle UMETA(DisplayName = "Idle"),
};