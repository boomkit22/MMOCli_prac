// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MMOComponent/MonsAttributeComponent.h"
#include "HUD/HUDMonsterComponent.h"
#include "Type.h"
#include "Components/BoxComponent.h"
// Sets default values

 //WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    //WeaponMesh->SetupAttachment(GetMesh(), TEXT("b_MF_Weapon_R")); // "WeaponSocketName"�� ������ ���� �̸����� �ٲߴϴ�.
    //// ���� �޽� ����
    //static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("/Game/Weapons/Blunt_SpikedClub/SK_Blunt_SpikedClub"));
    //if (WeaponAsset.Succeeded())
    //{
    //    WeaponMesh->SetSkeletalMesh(WeaponAsset.Object);
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Failed to load monster weapon skeletal mesh."));
    //}


AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));

    RootComponent = MeshComponent;
    CollisionBoxComponent->SetupAttachment(RootComponent);

    MonsAttributeComponent = CreateDefaultSubobject<UMonsAttributeComponent>(TEXT("MonsAttributeComponent"));
    MonsAttributeComponent->Init(100, FString("Monster"));

    HUDMonsterComponent = CreateDefaultSubobject<UHUDMonsterComponent>(TEXT("HUDMonsterComponent"));
    HUDMonsterComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay(); 
    GetCharacterMovement()->MaxWalkSpeed = 300.0f; // ��: 300 ����/��
	

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




void AMonster::SetMonsterProperties(MonsterInfo monsterInfo)
{
    MonsterType = static_cast<EMonsterType>(monsterInfo.Type);
    MonsterID = monsterInfo.MonsterID;


    switch(MonsterType)
    {
        case EMonsterType::EMT_Guardian:
        {
            MeshComponent->SetSkeletalMesh(GuardianMesh);
            CollisionBoxComponent->SetBoxExtent(GuardianCollisionExtent);
            AttackMontage = GuardianAttackMontage;
            DeathMontage = GuardianDeathMontage;
            MeshComponent->SetAnimInstanceClass(GuardianAnimBlueprint);
        }
        break;

        case EMonsterType::EMT_Spider:
        {
            MeshComponent->SetSkeletalMesh(SpiderMesh);
            CollisionBoxComponent->SetBoxExtent(GuardianCollisionExtent);
            AttackMontage = SpiderAttackMontage;
            DeathMontage = SpiderDeathMontage;
            MeshComponent->SetAnimInstanceClass(SpiderAnimBlueprint);
        }
        break;

        default:
            break;
    }
}

void AMonster::SetDestination(FVector destination)
{
    Destination = destination;
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

