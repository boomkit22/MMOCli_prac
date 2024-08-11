// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MMOComponent/MonsAttributeComponent.h"
#include "HUD/HUDMonsterComponent.h"
#include "Type.h"
#include "Components/BoxComponent.h"
#include "Items/Weapon.h"
#include "GameInstance/MMOGameInstance.h"

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

void AMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
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
            MonsAttributeComponent->Init(monsterInfo.Hp, FString("Guardian"));
            MeshComponent->SetSkeletalMesh(GuardianMesh);
            CollisionBoxComponent->SetBoxExtent(GuardianCollisionExtent);
            AttackMontage = GuardianAttackMontage;
            DeathMontage = GuardianDeathMontage;
            MeshComponent->SetAnimInstanceClass(GuardianAnimBlueprint);

            if (GuardianWeaponClass)
            {
                EquippedWeapon = UMMOGameInstance::GetMMOWorld()->SpawnActor<AWeapon>(GuardianWeaponClass);
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
            MonsAttributeComponent->Init(monsterInfo.Hp, FString("Spider"));
            MeshComponent->SetSkeletalMesh(SpiderMesh);
            CollisionBoxComponent->SetBoxExtent(SpiderCollisionExtent);
            AttackMontage = SpiderAttackMontage;
            DeathMontage = SpiderDeathMontage;
            MeshComponent->SetAnimInstanceClass(SpiderAnimBlueprint);

            // 위치 설정
            MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f)); // MeshComponent를 아래로 88 이동
            CollisionBoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 88.0f)); // CollisionBoxComponent를 위로 88 이동
        }
        break;

        default:
            break;
    }

    if (HUDMonsterComponent)
    {
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
        // Destination 및 CurrentLocation에서 Z값을 현재 액터의 Z 위치로 설정
        FVector ModifiedDestination = FVector(Destination.X, Destination.Y, CurrentLocation.Z);
        FVector Direction = (ModifiedDestination - CurrentLocation).GetSafeNormal();
        float Distance = FVector::Dist(ModifiedDestination, CurrentLocation);

        // 이동 속도 설정
        float Step = Speed * DeltaTime; // 이번 프레임에서 이동할 거리

        if (Distance > Step)
        {
            // 새 위치 계산
            FVector NewLocation = CurrentLocation + Direction * Step;
            SetActorLocation(NewLocation);

            // 이동하는 방향으로 부드러운 회전 설정
            FRotator CurrentRotation = GetActorRotation();

            FRotator TargetRotation = Direction.Rotation();
            //SetActorRotation(TargetRotation);
            FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime * 20.0f); // 회전 속도 조정 가능
            SetActorRotation(NewRotation);
        }
        else
        {
            // 목적지에 매우 가까워졌을 때, 목적지에 도달
            SetActorLocation(ModifiedDestination);
            //Destination = FVector::ZeroVector; // 도착 후 목적지 초기화
            UE_LOG(LogTemp, Warning, TEXT("EMS_Idle"));

            PathIndex++;
            if (PathIndex < Path.size())
            {
                SetDestination(FVector(Path[PathIndex].x, Path[PathIndex].y, CurrentLocation.Z));
            }
            else
            {
                Destination = FVector::ZeroVector;
                //MovingState = EMovingState::EMS_Idle;
            }
        }
    }
    else {
        MonsterState = EMonsterState::EMS_Idle;
    }
}

void AMonster::Attack()
{
    UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
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

void AMonster::StopMove()
{
    Destination = FVector::ZeroVector;
	MonsterState = EMonsterState::EMS_Idle;
}

int AMonster::GetType()
{
    return TYPE_MONSTER;
}

int64 AMonster::GetId()
{
    return MonsterID;
}

void AMonster::SetPath(FVector StartPos, uint16 startIndex, std::vector<Pos>& path)
{
    PathIndex = startIndex;
    //StartIndex = startIndex;
    Path = path;
    //UE_LOG(LogTemp, Warning, TEXT("SetPath %d"), Path.size());

    FVector CurrentLocation = GetActorLocation();
    float Distance = FVector::Dist(CurrentLocation, StartPos);

    // 일정 수치 이상 차이가 나면 , sync
    float Threshold = 500.0f;
    if (Distance > Threshold)
    {
        SetActorLocation(StartPos);
        //UE_LOG(LogTemp, Warning, TEXT("Character moved to StartPos due to large distance: %f"), Distance);
    }
}

