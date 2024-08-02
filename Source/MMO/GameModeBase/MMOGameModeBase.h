// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MMOGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API AMMOGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMMOGameModeBase();
	virtual void BeginPlay() override;

private:
	// 장애물 생성 및 맵 저장
	void ProcessMap();
	TArray<TArray<uint8>> ObstacleMaps;
	const int32 MapWidth = 12000;
	const int32 MapHeight = 12000;
	void GenerateObstacleMapData(UWorld* World);
	void SaveObstacleMapsToFile(const FString& FileName);
};
