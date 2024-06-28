// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GameCharacter.h"
#include "RemoteGameCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API ARemoteGameCharacter : public AGameCharacter
{
	GENERATED_BODY()
	
public:
	ARemoteGameCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void InitCharAttributeComponent(int32 Health, FString CharName, int32 Level) override;

protected:
	virtual void GetHit(int32 damage) override;



private:
	UPROPERTY(VisibleAnywhere)
	class UHUDRemoteCharacterComponent* HUDRemoteCharacterComponent;
};
