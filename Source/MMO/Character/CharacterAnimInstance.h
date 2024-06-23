// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterTypes.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class AGameCharacter* GameCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* CharacterMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EMovingState MovingState = EMovingState::EMS_Idle;
};
