// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/MonsterAnimInstance.h"
#include "Monsters/Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Monster = Cast<AMonster>(TryGetPawnOwner());
	
	//Character = Cast<ACharacter>(TryGetPawnOwner());
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (Monster)
	{
		MonsterState = Monster->MonsterState;
	}
}