#pragma once

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	EAS_Move UMETA(DisplayName = "Move"),
	EAS_Idle UMETA(DisplayName = "Idle"),
};