// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Monsters/Monster.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 500.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	// 무기 메시를 생성하고 로드합니다.
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_rSocket")); // "WeaponSocketName"을 생성한 소켓 이름으로 바꿉니다.
	// 무기 메쉬 설정

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponAsset(TEXT("/Game/Weapons/Blade_BlackKnight/SK_Blade_BlackKnight"));
	if (WeaponAsset.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(WeaponAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load weapon skeletal mesh."));
	}

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

}

// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AGameCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AGameCharacter::LookUp);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AGameCharacter::Attack);
	PlayerInputComponent->BindAction("SpawnMonster", IE_Pressed, this, &AGameCharacter::SpawnMonster);
	PlayerInputComponent->BindAction("MoveMonster", IE_Pressed, this, &AGameCharacter::MoveMonster);
}

void AGameCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
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
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	//PlayAttackMontage();
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
void AGameCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 Selection = FMath::RandRange(0, 1);
		FName SelectionName = FName();
		switch (Selection)
		{
		case 0:
		{
			SelectionName = FName("Combo1");
		}
		break;

		case 1:
		{
			SelectionName = FName("Combo1-1");
		}
		break;

		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SelectionName, AttackMontage);
	}
}

