// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"
class AGameCharacter;
/**
 * 
 */
UCLASS()
class MMO_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;


public:
	AGameCharacter* SpawnMyCharacter(int64 PlayerID, FVector SpawnLocation, uint16 Level, TCHAR* NickName);

public:
	// YourCharacterClass ������ TSubclassOf�� ����Ͽ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AActor> GameCharacterClass;
	

};
