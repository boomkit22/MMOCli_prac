// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Monsters/MonsterState.h"
#include "MonsterAnimInstance.generated.h"
/**
 * 
 */
UCLASS()
class MMO_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class AMonster* Monster;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* CharacterMovementComponent;

	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EMonsterState MonsterState = EMonsterState::EMS_Idle;
};
