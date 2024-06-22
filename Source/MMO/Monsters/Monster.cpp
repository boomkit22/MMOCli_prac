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
    GetCharacterMovement()->MaxWalkSpeed = 300.0f; // ��: 300 ����/��
	
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
    // ������ ���� (��: ĳ���� �������� 1000 ���� �Ÿ�)
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

        // �̵� �ӵ� ����
        float Speed = 300.0f; // ��: 300 ����/��
        float Step = Speed * DeltaTime; // �̹� �����ӿ��� �̵��� �Ÿ�

        if (Distance > Step)
        {
            // �� ��ġ ���
            FVector NewLocation = CurrentLocation + Direction * Step;
            SetActorLocation(NewLocation);

            // �̵��ϴ� �������� �ε巯�� ȸ�� ����
            FRotator CurrentRotation = GetActorRotation();
            FRotator TargetRotation = Direction.Rotation();
            FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime * 5.0f); // ȸ�� �ӵ� ���� ����
            SetActorRotation(NewRotation);
        }
        else
        {
            // �������� �ſ� ��������� ��, �������� ����
            SetActorLocation(Destination);
            Destination = FVector::ZeroVector; // ���� �� ������ �ʱ�ȭ
        }
    }
    else {
        MonsterState = EMonsterState::EAS_Idle;
    }
}

