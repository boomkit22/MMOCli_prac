// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/MMOGameModeBase.h"
#include "Character/GamePlayerController.h"
#include "Serialization/BufferArchive.h"


AMMOGameModeBase::AMMOGameModeBase()
{
    // Set default pawn class to none
    DefaultPawnClass = nullptr;
    
    // Use custom PlayerController class
    PlayerControllerClass = AGamePlayerController::StaticClass();
}

void AMMOGameModeBase::BeginPlay()
{
    Super::BeginPlay();
	

	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("ProcessMapStart"));
	ProcessMap();
	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("ProcessMapEnd"));
}

void AMMOGameModeBase::ProcessMap()
{
	//for (const FString& MapName : Maps)
	//{
	FString MapName = TEXT("LobbyMap");
	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, MapName);
	//�� �ε� �ϰ�
	//UGameplayStatics::OpenLevel(GetInstance(), FName(*MapName), false);
	UWorld* World = GetWorld();
	if (World)
	{
		//�� ������ ����
		GenerateObstacleMapData(World);
		//���Ϸ� ����
		FString FileName = MapName + TEXT(".dat");
		SaveObstacleMapsToFile(FileName);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("World is null"));
	}

	//}
}


void AMMOGameModeBase::GenerateObstacleMapData(UWorld* World)
{
	ObstacleMaps.Empty(); // 

	// 2���� �迭 �ʱ�ȭ
	ObstacleMaps.SetNum(MapHeight);
	for (int32 Y = 0; Y < MapHeight; ++Y)
	{
		ObstacleMaps[Y].SetNum(MapWidth);
		for (int32 X = 0; X < MapWidth; ++X)
		{

			FVector Start = FVector(X, Y, 2000); // ������
			FVector End = FVector(X, Y, 0);  // �Ʒ���

			FHitResult HitResult;

			bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel6); // 6�� Obstacle ä��
			// ��ֹ� �ִ� �� 1��
			if (bHit)
			{
				//DrawDebugLine(World, Start, End, FColor::Green, true);
				//GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Obstacle"));
				ObstacleMaps[Y][X] = 1;
			}
		}
	}
}

void AMMOGameModeBase::SaveObstacleMapsToFile(const FString& FileName)
{
	FBufferArchive ToBinary;

	for (int32 Y = 0; Y < MapHeight; ++Y)
	{
		for (int32 X = 0; X < MapWidth; ++X)
		{
			ToBinary << ObstacleMaps[Y][X];
		}
	}
	
	FString FilePath = FPaths::ProjectDir() / FileName;
	if (FFileHelper::SaveArrayToFile(ToBinary, *FilePath))
	{
		ToBinary.FlushCache();
		ToBinary.Empty();
	}

}