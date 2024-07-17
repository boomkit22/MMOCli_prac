// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyGameCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/MMOGameInstance.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "PacketMaker/GamePacketMaker.h"
#include "Portal/PortalBoxActor.h"
#include "HUD/MMOHUD.h"
#include "HUD/MMOOverlay.h"
#include "GameData.h"
#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "MMOComponent/CharAttributeComponent.h"


AMyGameCharacter::AMyGameCharacter()
{
	// Dont rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	// Top down camera
	// Create the camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Set the camera's angle for top-down view
	FRotator CameraRotation = FRotator(-60.0f, 0.0f, 0.0f); // Adjust the angle as needed
	CameraBoom->SetWorldRotation(CameraRotation);
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	// Create the camera and attach to boom
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom
	ViewCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm



	// 캡슐 컴포넌트 설정 포탈하고 overlap
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Overlap); // Och_Portal과 오버랩 설정
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyGameCharacter::OnPortalOverlapBegin);
}

void AMyGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(UMMOGameInstance::GetMMOWorld(), 0);
	if (PlayerController)
	{
		// 마우스 커서 보이게 설정
		UE_LOG(LogTemp, Warning, TEXT("PlayerController->bShowMouseCursor"));
		PlayerController->bShowMouseCursor = true;
	}

	if (PlayerController)
	{
		AMMOHUD* MMOHUD = Cast<AMMOHUD>(PlayerController->GetHUD());
		if (MMOHUD)
		{
			MMOOverlay = MMOHUD->GetMMOOverlay();
		}
	}
}

void AMyGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (AttackCooldownTimerHandle.IsValid())
	{
		UMMOGameInstance::GetMMOWorld()->GetTimerManager().ClearTimer(AttackCooldownTimerHandle);
	}
}

void AMyGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AMyGameCharacter::LeftMouseClick);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyGameCharacter::Attack);
	PlayerInputComponent->BindAction("ToLobby", IE_Pressed, this, &AMyGameCharacter::ToLobby);
}

void AMyGameCharacter::Attack()
{

	//TODO: Attack CoolTime
	if (bIsCool)
	{
		return;
	}

	bIsCool = true;
	UMMOGameInstance::GetMMOWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &AMyGameCharacter::ResetAttackCoolTime, AttackCoolTime, false);

	if (MovingState == EMovingState::EMS_Dead)
	{
		return;
	}

	hasHit = false;

	if (ActionState == EActionState::EAS_Unoccupied)
	{
		ActionState = EActionState::EAS_Attacking;

		StopMove();
		int32 SkillID = FMath::RandRange(0, 1);
		CPacket* Packet = CPacket::Alloc();
		FVector StartLocation = GetActorLocation();
		FRotator StartRotator = GetActorRotation();
		GamePacketMaker::MP_SC_REQ_CHARACTER_SKILL(Packet, StartLocation, StartRotator, SkillID);
		UMMOGameInstance::GetInstance()->SendPacket_GameServer(Packet);

		PlaySkill(SkillID);
	}
	//PlayAttackMontage();
}

void AMyGameCharacter::ResetAttackCoolTime()
{
	bIsCool = false;
}

void AMyGameCharacter::LeftMouseClick()
{
	if (ActionState == EActionState::EAS_Attacking)
	{
		return;
	}

	bool bHitSuccessful = false;
	FHitResult Hit;
	bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	CPacket* MoveReqPacket = CPacket::Alloc();
	//FRotat StartPosition = GetActorLocation();
	FRotator StartRotation = GetActorRotation();

	GamePacketMaker::MP_CS_REQ_CHARACTER_MOVE(MoveReqPacket, Hit.Location, StartRotation);
	UMMOGameInstance::GetInstance()->SendPacket_GameServer(MoveReqPacket);
}

void AMyGameCharacter::ToLobby()
{
	uint16 LobbyFieldID = FIELD_LOBBY;
	RequestFieldMove(LobbyFieldID);
}

void AMyGameCharacter::RequestFieldMove(uint16 fieldId)
{
	//TODO: 캐릭터 멈추고
	StopMove();
	if (PlayerController)
	{
		// disable input
		PlayerController->DisableInput(PlayerController);
	}

	CPacket* fieldMoveReqPacket = CPacket::Alloc();
	GamePacketMaker::MP_CS_REQ_FIELD_MOVE(fieldMoveReqPacket, fieldId);
	UMMOGameInstance::GetInstance()->SendPacket_GameServer(fieldMoveReqPacket);
}


void AMyGameCharacter::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Weapon Box Begin Overlap")));
	if (hasHit)
		return;

	IHittableInterface* AttackedCharacter = Cast<IHittableInterface>(OtherActor);
	if (AttackedCharacter)
	{
		hasHit = true;
		//AttackedCharacter->GetHit(20);
		CPacket* packet = CPacket::Alloc();
		//packet = CPacket::Alloc();
		int64 AttackerID = GetId();
		int32 AttackerType = GetType();
		int64 TargetID = AttackedCharacter->GetId();
		int32 TargetType = AttackedCharacter->GetType();
		GamePacketMaker::MP_CS_REQ_CHARACTER_ATTACK(packet, AttackerType, AttackerID, TargetType, TargetID);
		UMMOGameInstance::GetInstance()->SendPacket_GameServer(packet);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Damage Packet")));
	}
}

void AMyGameCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AttackEnd")));
}

void AMyGameCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		UE_LOG(LogTemp, Warning, TEXT("set weapon"));
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
	}
}

void AMyGameCharacter::EquipWeapon()
{
	Super::EquipWeapon();

	if (EquippedWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Set Weapon Overlap ")));
		UBoxComponent* WeaponBox = EquippedWeapon->GetWeaponBox();
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMyGameCharacter::OnWeaponBeginOverlap);
	}
}

void AMyGameCharacter::OnPortalOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// OchPortal 인지 확인 ECC_GameTraceChannel5
		if (OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel5)
		{

			APortalBoxActor* PortalBoxActor = Cast<APortalBoxActor>(OtherActor);
			if (PortalBoxActor)
			{
				uint16 fieldId = PortalBoxActor->FieldId;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("PortalBoxActor FieldId : %d"), fieldId));

				//TODO:  필드 이동 패킷 쏘기
				RequestFieldMove(fieldId);
			}
		}
	}
}

void AMyGameCharacter::GetHit(int32 damage)
{
	Super::GetHit(damage);

	if (MMOOverlay && CharAttributeComponent)
	{
		MMOOverlay->SetHealthBarPercent(CharAttributeComponent->GetHelathPercent());
	}
}
