// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MMOComponent/MonsAttributeComponent.h"
#include "HUD/HUDMonsterComponent.h"
#include "Type.h"
#include "Components/BoxComponent.h"
#include "Items/Weapon.h"
// Sets default values

 //WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    //WeaponMesh->SetupAttachment(GetMesh(), TEXT("b_MF_Weapon_R")); // "WeaponSocketName"을 생성한 소켓 이름으로 바꿉니다.
    //// 무기 메쉬 설정
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


    PrimaryActorTick.bCanEverTick = true;

    // 캡슐 컴포넌트를 루트 컴포넌트로 사용
    RootComponent = GetCapsuleComponent();

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxComponent"));

    // MeshComponent를 CapsuleComponent에
    MeshComponent->SetupAttachment(RootComponent);
    CollisionBoxComponent->SetupAttachment(MeshComponent);

    // MeshComponent의 초기 회전을 설정
    FRotator InitialRotation = FRotator(0.0f, -90.0f, 0.0f);
    MeshComponent->SetRelativeRotation(InitialRotation);

    HUDMonsterComponent = CreateDefaultSubobject<UHUDMonsterComponent>(TEXT("HUDMonsterComponent"));
    HUDMonsterComponent->SetupAttachment(MeshComponent);

    MonsAttributeComponent = CreateDefaultSubobject<UMonsAttributeComponent>(TEXT("MonsAttributeComponent"));

    // 콜리전 설정
    CollisionBoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); // Och_DamageReceive 채널
    CollisionBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECR_Overlap); // Och_DamageGiver 채널과만 오버랩
    CollisionBoxComponent->SetGenerateOverlapEvents(true); // 오버랩 이벤트 생성 설정
    CollisionBoxComponent->SetHiddenInGame(false);
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




void AMonster::SetMonsterProperties(MonsterInfo monsterInfo)
{
    MonsterType = static_cast<EMonsterType>(monsterInfo.Type);
    MonsterID = monsterInfo.MonsterID;


    switch(MonsterType)
    {
        case EMonsterType::EMT_Guardian:
        {
            MonsAttributeComponent->Init(100, FString("Guardian"));
            MeshComponent->SetSkeletalMesh(GuardianMesh);
            CollisionBoxComponent->SetBoxExtent(GuardianCollisionExtent);
            AttackMontage = GuardianAttackMontage;
            DeathMontage = GuardianDeathMontage;
            MeshComponent->SetAnimInstanceClass(GuardianAnimBlueprint);

            if (GuardianWeaponClass)
            {
                EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(GuardianWeaponClass);
                if (EquippedWeapon)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("monster wepaon")));
                    EquippedWeapon->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("b_MF_Weapon_R"));
                }
            }

            // 위치 설정
            MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f)); // MeshComponent를 아래로 88 이동
            CollisionBoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 88.0f)); // CollisionBoxComponent를 위로 88 이동
        }
        break;

        case EMonsterType::EMT_Spider:
        {
            MonsAttributeComponent->Init(100, FString("Spider"));
            MeshComponent->SetSkeletalMesh(SpiderMesh);
            CollisionBoxComponent->SetBoxExtent(SpiderCollisionExtent);
            AttackMontage = SpiderAttackMontage;
            DeathMontage = SpiderDeathMontage;
            MeshComponent->SetAnimInstanceClass(SpiderAnimBlueprint);
        }
        break;

        default:
            break;
    }

    if (HUDMonsterComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Set Monster HUD Component."));
        HUDMonsterComponent->SetMonsterName(MonsAttributeComponent->GetMonsterName());
        HUDMonsterComponent->SetHealthPercent(MonsAttributeComponent->GetHelathPercent());
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

