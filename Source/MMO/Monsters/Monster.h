// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monsters/MonsterState.h"
#include "Interface/HittableInterface.h"
#include "Monster.generated.h"



class UHUDMonsterComponent;

UCLASS()
class MMO_API AMonster : public ACharacter, public IHittableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//test
	void SetDestination();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector Destination;

	void MoveToDestination(float DeltaTime);

	EMonsterState MonsterState = EMonsterState::EMS_Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComponent;

public:
	void Attack();
	void Death();
	void GetHit(int damage);

private:
	UPROPERTY(VisibleAnywhere)
	class UMonsAttributeComponent* MonsAttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UHUDMonsterComponent* HUDMonsterComponent;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	/**
	* Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	// IHittableInterface을(를) 통해 상속됨
	int GetType() override;
	int64 GetId() override;

private:
	int64 MonsterID;
};

