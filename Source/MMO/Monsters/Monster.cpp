// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay(); 
    GetCharacterMovement()->MaxWalkSpeed = 300.0f; // 예: 300 유닛/초
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    MoveToDestination(DeltaTime);
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMonster::SetDestination()
{
    // 목적지 설정 (예: 캐릭터 앞쪽으로 1000 유닛 거리)
    int32 Selection = FMath::RandRange(0, 4);

    switch (Selection)
    {
    case 0:
    Destination = GetActorLocation() - GetActorRightVector() * 1000.0f;
    break;
    case 1:
    Destination = GetActorLocation() + GetActorForwardVector() * 1000.0f;
	break;
    case 2:
	Destination = GetActorLocation() - GetActorForwardVector() * 1000.0f;
    break;
	case 3:
    Destination = GetActorLocation() + GetActorRightVector() * 1000.0f;
	break;


    }
    //Destination = GetActorLocation() + GetActorForwardVector() * 1000.0f;
}


void AMonster::MoveToDestination(float DeltaTime)
{
    if (!Destination.IsZero())
    {
        MonsterState = EMonsterState::EAS_Move;
        FVector CurrentLocation = GetActorLocation();
        FVector Direction = (Destination - CurrentLocation).GetSafeNormal();
        float Distance = FVector::Dist(Destination, CurrentLocation);

        // 이동 속도 설정
        float Speed = 300.0f; // 예: 300 유닛/초
        float Step = Speed * DeltaTime; // 이번 프레임에서 이동할 거리

        if (Distance > Step)
        {
            // 새 위치 계산
            FVector NewLocation = CurrentLocation + Direction * Step;
            SetActorLocation(NewLocation);

            // 이동하는 방향으로 부드러운 회전 설정
            FRotator CurrentRotation = GetActorRotation();
            FRotator TargetRotation = Direction.Rotation();
            FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime * 5.0f); // 회전 속도 조정 가능
            SetActorRotation(NewRotation);
        }
        else
        {
            // 목적지에 매우 가까워졌을 때, 목적지에 도달
            SetActorLocation(Destination);
            Destination = FVector::ZeroVector; // 도착 후 목적지 초기화
        }
    }
    else {
        MonsterState = EMonsterState::EAS_Idle;
    }
}

