// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GameCharacter.h"
#include "MyGameCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API AMyGameCharacter : public AGameCharacter
{
	GENERATED_BODY()
	friend class UMMOGameInstance;

public:
	AMyGameCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;



protected:
	class APlayerController* PlayerController;


protected:
	UPROPERTY()
	UMMOOverlay* MMOOverlay;

private:
	float AttackCoolTime = 0.6f;
	bool bIsCool = false;
	FTimerHandle AttackCooldownTimerHandle;
	void ResetAttackCoolTime();

private:
	//virtual void MoveToDestination(float DeltaTime) override;
	void LeftMouseClick();
	void Attack();
	void ToLobby();
	void RequestFieldMove(uint16 fieldIds);

private:
	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool hasHit = false;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	virtual void EquipWeapon() override;

	UFUNCTION()
	void OnPortalOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void GetHit(int32 damage) override;

	uint16 PathSize = 0;
	uint16 CurrentPathIndex = 0;
};
