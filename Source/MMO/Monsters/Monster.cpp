// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MMOComponent/MonsAttributeComponent.h"
#include "HUD/HUDMonsterComponent.h"
#include "Type.h"
// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //CollisionComponent = GetCapsuleComponent();
    //CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Monstesr채널 생성
    //CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Ignore); // Player 채널 무시
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(GetMesh(), TEXT("b_MF_Weapon_R")); // "WeaponSocketName"을 생성한 소켓 이름으로 바꿉니다.
    // 무기 메쉬 설정
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("/Game/Weapons/Blunt_SpikedClub/SK_Blunt_SpikedClub"));
    if (WeaponAsset.Succeeded())
    {
        WeaponMesh->SetSkeletalMesh(WeaponAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load monster weapon skeletal mesh."));
    }

    MonsAttributeComponent = CreateDefaultSubobject<UMonsAttributeComponent>(TEXT("MonsAttributeComponent"));
    MonsAttributeComponent->Init(100, FString("Guardian"));

    HUDMonsterComponent = CreateDefaultSubobject<UHUDMonsterComponent>(TEXT("HUDMonsterComponent"));
    HUDMonsterComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay(); 
    GetCharacterMovement()->MaxWalkSpeed = 300.0f; // 예: 300 유닛/초
	

    if(HUDMonsterComponent)
	{
        UE_LOG(LogTemp, Error, TEXT("Set Monster HUD Component."));
        HUDMonsterComponent->SetMonsterName(MonsAttributeComponent->GetMonsterName());
        HUDMonsterComponent->SetHealthPercent(MonsAttributeComponent->GetHelathPercent());
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Monster HUD Component is nullptr."));
    }
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
    if(MonsterState == EMonsterState::EMS_Dead)
	{
		return;
	}

    if (!Destination.IsZero())
    {
        MonsterState = EMonsterState::EMS_Move;
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
        MonsterState = EMonsterState::EMS_Idle;
    }
}

void AMonster::Attack()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && AttackMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack"));
        AnimInstance->Montage_Play(AttackMontage);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Attack error"));
    }
}

void AMonster::Death()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && DeathMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Death"));
        AnimInstance->Montage_Play(DeathMontage);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Death error"));
    }
    MonsterState = EMonsterState::EMS_Dead;
    if(MonsterState == EMonsterState::EMS_Dead)
    {
        UE_LOG(LogTemp, Warning, TEXT("DEad"));
	}
    SetLifeSpan(5.0f);
}

void AMonster::GetHit(int damage)
{
    UE_LOG(LogTemp, Warning, TEXT("Monster Hit"));
    MonsAttributeComponent->GetDamage(damage);
    HUDMonsterComponent->SetHealthPercent(MonsAttributeComponent->GetHelathPercent());
    if (!MonsAttributeComponent->IsAlive())
	{
        Death();
	}
}

int AMonster::GetType()
{
    return TYPE_MONSTER;
}

int64 AMonster::GetId()
{
    return MonsterID;
}

