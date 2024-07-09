// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

//#include "GameInstacne/MMOGameInstacne.h""

// Sets default values
AGameCharacter::AGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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
	// Create the camera and attach to boom
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom
	ViewCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Set the camera's angle for top-down view
	FRotator CameraRotation = FRotator(-60.0f, 0.0f, 0.0f); // Adjust the angle as needed
	CameraBoom->SetWorldRotation(CameraRotation);

	// 콜리전 컴포넌트 설정
	//CollisionComponent = GetCapsuleComponent();
	//CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2); // Player 채널 설정
	//CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore); // Monster 채널 무시

	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(GetRootComponent());
	//CameraBoom->TargetArmLength = 500.f;

	//ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	//ViewCamera->SetupAttachment(CameraBoom);

	//TODO: 무기 생성 및 메시 가져와서 hand_rSocket에 붙이기
	



	
	// 무기 메시를 생성하고 로드합니다.
	//WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	//WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket")); // "WeaponSocketName"을 생성한 소켓 이름으로 바꿉니다.
	// 무기 메쉬 설정

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("/Game/Weapons/Blade_BlackKnight/SK_Blade_BlackKnight"));
	//if (WeaponAsset.Succeeded())
	//{
	//	WeaponMesh->SetSkeletalMesh(WeaponAsset.Object);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Failed to load weapon skeletal mesh."));
	//}

	//test
	static ConstructorHelpers::FClassFinder<ACharacter> MonsterBPClass(TEXT("/Game/Blueprint/Monsters/BP_Monster"));
	if (MonsterBPClass.Succeeded())
	{
		MonsterClass = MonsterBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Monster class loaded."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load monster class."));
	}

	DamageCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponBox"));
	DamageCapsule->SetupAttachment(GetRootComponent());

	// 캐릭터 속성 컴포넌트 생성
	CharAttributeComponent = CreateDefaultSubobject<UCharAttributeComponent>(TEXT("CharAttribute"));
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
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

	if (WeaponClass)
	{
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (EquippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("EquippedWeapon is spawned."))
			EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
		}
	}
	 DamageCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGameCharacter::OnBoxBeginOverlap);

	 if (EquippedWeapon)
	 {
		 UBoxComponent * WeaponBox = EquippedWeapon->GetWeaponBox();
		 WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		 WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AGameCharacter::OnWeaponBeginOverlap);
	 }

	
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToDestination(DeltaTime);
}

// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAxis("MoveForward", this, &AGameCharacter::MoveForward);
	//PlayerInputComponent->BindAxis("MoveRight", this, &AGameCharacter::MoveRight);
	//PlayerInputComponent->BindAxis("Turn", this, &AGameCharacter::Turn);
	//PlayerInputComponent->BindAxis("LookUp", this, &AGameCharacter::LookUp);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AGameCharacter::LeftMouseClick);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGameCharacter::Attack);
	PlayerInputComponent->BindAction("SpawnMonster", IE_Pressed, this, &AGameCharacter::SpawnMonster);
	PlayerInputComponent->BindAction("MoveMonster", IE_Pressed, this, &AGameCharacter::MoveMonster);
	PlayerInputComponent->BindAction("MonsterAttack", IE_Pressed, this, &AGameCharacter::MonsterAttack);
	PlayerInputComponent->BindAction("Death", IE_Pressed, this, &AGameCharacter::Death);
	PlayerInputComponent->BindAction("MonsterDeath", IE_Pressed, this, &AGameCharacter::MonsterDeath);
	PlayerInputComponent->BindAction("MonsterDamage", IE_Pressed, this, &AGameCharacter::MonsterDamage);
	PlayerInputComponent->BindAction("DamageTest", IE_Pressed, this, &AGameCharacter::DamageTest);
	PlayerInputComponent->BindAction("SpawnOtherCharacter", IE_Pressed, this, &AGameCharacter::SpawnOtherCharacter);
	PlayerInputComponent->BindAction("RecoverHealth", IE_Pressed, this, &AGameCharacter::RecoverHealth);
}

void AGameCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AGameCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		UE_LOG(LogTemp, Warning, TEXT("set weapon"));
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);

		//EquippedWeapon->SetWeaponCollisionEnabled(CollisionEnabled);
	}
}

void AGameCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("1 Character OnBoxBeginOverlap : %s"), *OtherActor->GetName());

	//AGameCharacter* AttackingCharacter = Cast<AGameCharacter>(GetOwner());
	//IHittableInterface* AttackingCharacter = Cast<IHittableInterface>(OtherActor);

	//UE_LOG(LogTemp, Warning, TEXT("Overlap Character Attacked %s"), *OtherActor->GetName());

	//if (AttackingCharacter)
	//{
		//UE_LOG(LogTemp, Warning, TEXT("Overlap Character Attacked Attacker Id : %lld, Attacker Type : %d"), AttackingCharacter->GetId(), AttackingCharacter->GetType());
	/*	AGameCharacter* AttackedCharacter = Cast<AGameCharacter>(OtherActor);
		AttackedCharacter->GetHit(20);
		DrawDebugSphere(GetWorld(), SweepResult.ImpactPoint, 10.0f, 10, FColor::Red, false, 1.0f);*/
	//}
}

void AGameCharacter::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:
	//UE_LOG(LogTemp, Warning, TEXT("Weapon Overlap with: %s"), *OtherActor->GetName());
	//AGameCharacter* AttacedCharacter = Cast<AGameCharacter>(OtherActor);
	//if (AttacedCharacter)
	//{
	//	AttacedCharacter->GetHit(20);
	//}
	EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IHittableInterface* AttackedCharacter = Cast<IHittableInterface>(OtherActor);
	if(AttackedCharacter)
	{
		
		//AttackedCharacter->GetHit(20);

		CPacket* packet = CPacket::Alloc();
		//packet = CPacket::Alloc();
		
		int64 AttackerID = GetId();
		int32 AttackerType = GetType();
		int64 TargetID = AttackedCharacter->GetId();
		int32 TargetType = AttackedCharacter->GetType();
		int64 Damage = 5;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("attacker : %lld, target : %lld"), AttackerID, TargetID));
		//UE_LOG(LogTemp, Warning, TEXT("attacker : %lld, target : %lld"), attackInfo.AttackerID, attackInfo.TargetID);

		GamePacketMaker::MP_CS_REQ_CHARACTER_ATTACK(packet, AttackerType, AttackerID, TargetType, TargetID, Damage);
		UMMOGameInstance::GetInstance()->SendPacket_GameServer(packet);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Damage Packet")));

	}

}

void AGameCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		//find out which way is forward
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		/*FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);*/
	}
}

void AGameCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		//find out which way is right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

		/*FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);*/
	}
}

void AGameCharacter::Turn(float Value)
{
	// changing yaw
	AddControllerYawInput(Value);
}

void AGameCharacter::LookUp(float Value)
{
	// changing pitch
	AddControllerPitchInput(Value);
}

void AGameCharacter::Attack()
{
	//TODO: Attack CoolTime
	if (bIsCool)
	{
		return;
	}

	bIsCool = true;
	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &AGameCharacter::ResetAttackCoolTime, AttackCoolTime, false);

	if(MovingState == EMovingState::EMS_Dead)
	{
		return;
	}


	if (ActionState == EActionState::EAS_Unoccupied)
	{
		StopMove();
		int32 SkillID = FMath::RandRange(0, 1);
		CPacket* Packet = CPacket::Alloc();
		FRotator StartRotator = GetActorRotation();
		GamePacketMaker::MP_SC_REQ_CHARACTER_SKILL(Packet, StartRotator, SkillID);
		UMMOGameInstance::GetInstance()->SendPacket_GameServer(Packet);
		
		PlaySkill(SkillID);
		ActionState = EActionState::EAS_Attacking;
	}
	//PlayAttackMontage();
}

void AGameCharacter::LeftMouseClick()
{
	bool bHitSuccessful = false;
	FHitResult Hit;
	bHitSuccessful = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	
	CPacket* MoveReqPacket = CPacket::Alloc();
	FRotator StartRotation = GetActorRotation();
	GamePacketMaker::MP_CS_REQ_CHARACTER_MOVE(MoveReqPacket, Hit.Location, StartRotation);
	UMMOGameInstance::GetInstance()->SendPacket_GameServer(MoveReqPacket);

	/*if (bHitSuccessful)
	{
		Destination = Hit.Location;
		UE_LOG(LogTemp, Warning, TEXT("Destination: %s"), *Destination.ToString());
	}*/
}

void AGameCharacter::SpawnMonster()
{
	if (MonsterClass != nullptr)
	{
		// 몬스터를 스폰할 위치와 회전 값 설정
		FVector SpawnLocation = GetActorLocation() + FVector(10.0f, 0.0f, 0.0f);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// 월드에서 몬스터를 스폰
		SpawnedMonster = Cast<AMonster>(GetWorld()->SpawnActor<ACharacter>(MonsterClass, SpawnLocation, SpawnRotation));
	}
}
void AGameCharacter::MoveMonster()
{
	if (SpawnedMonster != nullptr)
	{
		SpawnedMonster->SetDestination();
	}
}

void AGameCharacter::MonsterAttack()
{
	if(SpawnedMonster != nullptr)
	{
		SpawnedMonster->Attack();
	}
}

void AGameCharacter::MonsterDeath()
{
	if (SpawnedMonster != nullptr)
	{
		SpawnedMonster->Death();
	}
}

void AGameCharacter::MonsterDamage()
{
	if (SpawnedMonster != nullptr)
	{
		SpawnedMonster->GetHit(20);
	}
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
		// Destination 및 CurrentLocation에서 Z값을 현재 액터의 Z 위치로 설정
		FVector ModifiedDestination = FVector(Destination.X, Destination.Y, CurrentLocation.Z);
		FVector Direction = (ModifiedDestination - CurrentLocation).GetSafeNormal();
		float Distance = FVector::Dist(ModifiedDestination, CurrentLocation);

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
			//SetActorRotation(TargetRotation);
			FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime * 20.0f); // 회전 속도 조정 가능
			SetActorRotation(NewRotation);
		}
		else
		{
			// 목적지에 매우 가까워졌을 때, 목적지에 도달
			SetActorLocation(ModifiedDestination);
			Destination = FVector::ZeroVector; // 도착 후 목적지 초기화
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

void AGameCharacter::DamageTest()
{
	GetHit(20);
}

void AGameCharacter::SetDestination(FVector Dest)
{
	Destination = Dest;
}

void AGameCharacter::SpawnOtherCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("SPawn Other Character"));

	//CPacket* packet;
	//packet = CPacket::Alloc();
	//SpawnOtherCharacterInfo spawnOtherCharacterInfo;
	//spawnOtherCharacterInfo.SpawnLocation = GetActorLocation() + FVector{ 100, 100, 0 };
	//spawnOtherCharacterInfo.PlayerID = 1;
	//spawnOtherCharacterInfo.Level = 1;
	//FString NickName = "TestCharacter";
	//FCString::Strcpy(spawnOtherCharacterInfo.NickName, *NickName);

	//*packet << spawnOtherCharacterInfo;
	//UMMOGameInstance::GetInstance()->HandleSpawnOhterCharacter(packet);
	//CPacket::Free(packet);
}

void AGameCharacter::RecoverHealth()
{
	CharAttributeComponent->RecoverHealth(100);
	if (MMOOverlay && CharAttributeComponent)
	{
		MMOOverlay->SetHealthBarPercent(CharAttributeComponent->GetHelathPercent());
	}
}

void AGameCharacter::InitCharAttributeComponent(int32 Health, FString CharName, int32 Level)
{
	CharAttributeComponent->Init(Health, CharName, Level);
}

void AGameCharacter::ResetAttackCoolTime()
{
	bIsCool = false;
}

int AGameCharacter::GetType()
{
	return TYPE_PLAYER;
}

int64 AGameCharacter::GetId()
{
	return PlayerID;
}

void AGameCharacter::GetHit(int32 damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Character Hit"));

	CharAttributeComponent->GetDamage(damage);
	if (MMOOverlay && CharAttributeComponent)
	{
		MMOOverlay->SetHealthBarPercent(CharAttributeComponent->GetHelathPercent());
	}

	/*HUDCharacterComponent->SetHealthPercent(MonsAttributeComponent->GetHelathPercent());
	*/
	if (!CharAttributeComponent->IsAlive())
	{
		Death();
	}
}
