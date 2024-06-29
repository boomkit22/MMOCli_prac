// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "Interface/HittableInterface.h"
#include "GameCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UMMOOverlay;
class AWeapon;

UCLASS()
class MMO_API AGameCharacter : public ACharacter, public IHittableInterface
{
	friend class UMMOGameInstance;
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);



protected:
	//input
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Attack();
	void LeftMouseClick();
	/**
	*Play montage function
	*/
	void PlayAttackMontage();
	
protected:
	EActionState ActionState = EActionState::EAS_Unoccupied;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;


	UPROPERTY(VisibleAnywhere)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass; 

	/**
	* Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

public:
	//Test
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<ACharacter> MonsterClass;  // 몬스터 블루프린트 클래스를 참조하는 변수

	void SpawnMonster();
	void MoveMonster();
	void MonsterAttack();
	class AMonster* SpawnedMonster;
	

public:
	FVector Destination;
	class APlayerController* PlayerController;
	
	EMovingState MovingState = EMovingState::EMS_Idle;
	void MoveToDestination(float DeltaTime);
	void StopMove();
	void Death();
	void MonsterDeath();
	void MonsterDamage();
	void DamageTest();
	void SetDestination(FVector Dest);
	void SpawnOtherCharacter();

protected:
	virtual void GetHit(int32 damage);

public:
	virtual void InitCharAttributeComponent(int32 Health, FString CharName, int32 Level);
	
protected:
	UPROPERTY(VisibleAnywhere)
	class UCharAttributeComponent* CharAttributeComponent;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Collision")

	class UCapsuleComponent* DamageCapsule;

private:
	UPROPERTY()
	UMMOOverlay* MMOOverlay;

	private:
		float AttackCoolTime = 0.6f;
		bool bIsCool = false;
		FTimerHandle AttackCooldownTimerHandle;

		void ResetAttackCoolTime();

public:
		FORCEINLINE int64 GetPlayerID() const { return PlayerID; }
		FORCEINLINE void SetPlayerID(int64 NewPlayerID) { PlayerID = NewPlayerID; }

protected:
		int64 PlayerID;


		// IHittableInterface을(를) 통해 상속됨
		int GetType() override;

		int64 GetId() override;

};
