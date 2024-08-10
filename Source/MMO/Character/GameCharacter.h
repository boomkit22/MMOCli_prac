// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/CharacterTypes.h"
#include "Interface/HittableInterface.h"
#include <vector>
#include "Type.h"

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
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		void Initialize(ECharacterClassType CharacterClassType);
	
protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
	
public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;
	
	
	
	
	
	
	
	
protected:
		/**
		*Play montage function
		*/
		void PlaySkill(int SkillID);
		
protected:
		EActionState ActionState = EActionState::EAS_Unoccupied;
	
	
	
		UPROPERTY(VisibleAnywhere)
		AWeapon* EquippedWeapon;
	
	
		UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AWeapon> SwordClass;
	
		UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AWeapon> AxeClass;
	
	
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

	class AMonster* SpawnedMonster;
	

public:
	FVector Destination;
	
	EMovingState MovingState = EMovingState::EMS_Idle;
	virtual void MoveToDestination(float DeltaTime);
	void StopMove();
	void Death();
	void SetDestination(FVector Dest);
	

protected:
	virtual void GetHit(int32 damage);
	virtual void SetLevel(uint16 level);

public:
	virtual void InitCharAttributeComponent(int32 Health, FString CharName, uint16 Level, uint32 Exp);
	virtual void InitCharAttributeComponent(int32 Health, FString CharName, uint16 Level);

	
protected:
	UPROPERTY(VisibleAnywhere)
	class UCharAttributeComponent* CharAttributeComponent;
	
public:
	FORCEINLINE UCharAttributeComponent* GetCharAttributeComponent() { return CharAttributeComponent; }



public:
		FORCEINLINE int64 GetPlayerID() const { return PlayerID; }
		FORCEINLINE void SetPlayerID(int64 NewPlayerID) { PlayerID = NewPlayerID; }

protected:
		int64 PlayerID;


		// IHittableInterface을(를) 통해 상속됨
		int GetType() override;

		int64 GetId() override;


protected:
		UPROPERTY(EditAnywhere, Category = "Character")
		ECharacterClassType CharacterClassType;


protected:
	//Act By Class
	virtual void EquipWeapon();

protected:
	void SetPath(FVector StartPos, uint16 startIndex, std::vector<Pos>& path);
	//uint16 StartIndex;
	std::vector<Pos> Path;
	uint16 PathIndex = 0;

};
