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
class AWeapon;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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
	void SetDestination(FVector Destination);

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
	void StopMove();

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
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBoxComponent;

	UPROPERTY(VisibleAnywhere)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Guardian")
	TSubclassOf<AWeapon> GuardianWeaponClass;

	// guardian
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guardian")
	USkeletalMesh* GuardianMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guardian")
	FVector GuardianCollisionExtent;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guardian")
	UAnimMontage* GuardianAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guardian")
	UAnimMontage* GuardianDeathMontage;

	// Animation Blueprint classes (to be set in Blueprints)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TSubclassOf<UAnimInstance> GuardianAnimBlueprint;


	// spider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider")
	USkeletalMesh* SpiderMesh;

	FVector SpiderCollisionExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider")
	UAnimMontage* SpiderAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider")
	UAnimMontage* SpiderDeathMontage;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TSubclassOf<UAnimInstance> SpiderAnimBlueprint;

protected:
	UAnimMontage* AttackMontage;
	UAnimMontage* DeathMontage;
	


private:
	int64 MonsterID;
	int64 Speed = 200.f;
};

