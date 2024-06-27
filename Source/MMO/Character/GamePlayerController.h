// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

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
	void SpawnMyCharacter(FVector SpawnLocation);

public:
	// YourCharacterClass ������ TSubclassOf�� ����Ͽ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AActor> GameCharacterClass;
	

};
