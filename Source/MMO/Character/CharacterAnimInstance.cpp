// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterAnimInstance.h"
#include "Character/GameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	GameCharacter = Cast<AGameCharacter>(TryGetPawnOwner());
	if (GameCharacter)
	{
		CharacterMovementComponent = GameCharacter->GetCharacterMovement();
	}
	//Character = Cast<ACharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (CharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovementComponent->Velocity);
	}
}
