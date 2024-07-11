// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monsters/MonsterState.h"
#include "Interface/HittableInterface.h"
#include "Type.h"

#include "Monster.generated.h"
#define MONSTER_TYPE_GUARDIAN 1
#define MONSTER_TYPE_SPIDER 2


class UHUDMonsterComponent;
class USkeletalMeshComponent;
class UBoxComponent;

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
	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category = "Type")
	EMonsterType MonsterType;

	void SetMonsterProperties(MonsterInfo monsterInfo);

	//test
	void SetDestination();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector Destination;

	void MoveToDestination(float DeltaTime);

	EMonsterState MonsterState = EMonsterState::EMS_Idle;

	UPROPERTY(VisibleAnywhere)
	UHUDMonsterComponent* HUDMonsterComponent;

public:
	void Attack();
	void Death();
	void GetHit(int damage);

private:
	UPROPERTY(VisibleAnywhere)
	class UMonsAttributeComponent* MonsAttributeComponent;

	/**
	* Animation montages
	*/
	

	// IHittableInterface을(를) 통해 상속됨
	int GetType() override;
	int64 GetId() override;

protected:
	// guardian
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMesh* GuardianMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FVector GuardianCollisionExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* GuardianWeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* GuardianAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* GuardianDeathMontage;
	// spider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMesh* SpiderMesh;

	FVector SpiderCollisionExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* SpiderAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* SpiderDeathMontage;


protected:
	UAnimMontage* AttackMontage;
	UAnimMontage* DeathMontage;
	
	USkeletalMeshComponent* MeshComponent;
	UBoxComponent* CollisionBoxComponent;

private:
	int64 MonsterID;
};

