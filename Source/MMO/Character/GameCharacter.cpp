// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Monsters/Monster.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimSequence.h"
#include "MMOComponent/CharAttributeComponent.h"
#include "HUD/MMOHUD.h"
#include "HUD/MMOOverlay.h"
#include "GameInstance/MMOGameInstance.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "PacketMaker/GamePacketMaker.h"
#include "Items/Weapon.h"
#include "Portal/PortalBoxActor.h"

//#include "GameInstacne/MMOGameInstacne.h""

AGameCharacter::AGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĳ���� �Ӽ� ������Ʈ ����
	CharAttributeComponent = CreateDefaultSubobject<UCharAttributeComponent>(TEXT("CharAttribute"));
}

void AGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EndPlay")));


	if (EquippedWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EquippedWeapon is destroyed.")));
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}
}

																												
void AGameCharacter::Initialize(ECharacterClassType characterClassType)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Initialize %d"), (int)characterClassType));

	CharacterClassType = characterClassType;
	EquipWeapon();

}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToDestination(DeltaTime);
}

void AGameCharacter::PlaySkill(int SkillID)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{ 
		ECharacterSkill Skill = static_cast<ECharacterSkill>(SkillID);
		FName SelectionName = FName();
		AnimInstance->Montage_Play(AttackMontage);
		switch (Skill)
		{
		case ECharacterSkill::ECS_Combo1:
		{
			SelectionName = "Comobo";
		}
		break;

		case ECharacterSkill::ECS_Combo1_1:
		{
			SelectionName = "Combo1-1";
		}
		break;


		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SelectionName, AttackMontage);
	}
}

void AGameCharacter::MoveToDestination(float DeltaTime)
{
	if(MovingState == EMovingState::EMS_Dead)
	{
		return;
	}

	if (!Destination.IsZero())
	{
		MovingState = EMovingState::EMS_Move;
		FVector CurrentLocation = GetActorLocation();
		// Destination �� CurrentLocation���� Z���� ���� ������ Z ��ġ�� ����
		FVector ModifiedDestination = FVector(Destination.X, Destination.Y, CurrentLocation.Z);
		FVector Direction = (ModifiedDestination - CurrentLocation).GetSafeNormal();
		float Distance = FVector::Dist(ModifiedDestination, CurrentLocation);

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
			//SetActorRotation(TargetRotation);
			FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime * 20.0f); // ȸ�� �ӵ� ���� ����
			SetActorRotation(NewRotation);
		}
		else
		{
			// �������� �ſ� ��������� ��, �������� ����
			SetActorLocation(ModifiedDestination);
			Destination = FVector::ZeroVector; // ���� �� ������ �ʱ�ȭ
			UE_LOG(LogTemp, Warning, TEXT("EMS_Idle"));
		}
	}
	else {
		MovingState = EMovingState::EMS_Idle;
	}
}

void AGameCharacter::StopMove()
{
	Destination = FVector::ZeroVector;
	MovingState = EMovingState::EMS_Idle;
}

void AGameCharacter::Death()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
	MovingState = EMovingState::EMS_Dead;

	UE_LOG(LogTemp, Warning, TEXT("Character is Dead"));
}

void AGameCharacter::SetDestination(FVector Dest)
{
	Destination = Dest;
}

void AGameCharacter::InitCharAttributeComponent(int32 Health, FString CharName, int32 Level)
{
	CharAttributeComponent->Init(Health, CharName, Level);
}

int AGameCharacter::GetType()
{
	return TYPE_PLAYER;
}

int64 AGameCharacter::GetId()
{
	return PlayerID;
}

void AGameCharacter::EquipWeapon()
{
	switch (CharacterClassType)
	{
		case ECharacterClassType::CCT_Sword:
		{
			if (SwordClass)
			{
				EquippedWeapon = UMMOGameInstance::GetMMOWorld()->SpawnActor<AWeapon>(SwordClass);
				if (EquippedWeapon)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EquippedWeapon is spawned CCT_Sword.")));
					EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
				}
			}
		}
		break;

		case ECharacterClassType::CCT_Axe:
		{
			if (AxeClass)
			{
				EquippedWeapon = UMMOGameInstance::GetMMOWorld()->SpawnActor<AWeapon>(AxeClass);
				if (EquippedWeapon)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("EquippedWeapon is spawned. CCT_Axe")));
					EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
				}
			}
		}
		break;

		default:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("spawn weapon defualt")));

			break;
	}
}

void AGameCharacter::GetHit(int32 damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Character Hit"));

	CharAttributeComponent->GetDamage(damage);
}
