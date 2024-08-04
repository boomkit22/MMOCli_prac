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
	//ProcessMap();
	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("ProcessMapEnd"));
}

void AMMOGameModeBase::ProcessMap()
{
	//for (const FString& MapName : Maps)
	//{
	FString MapName = TEXT("LobbyMap");
	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, MapName);
	//맵 로딩 하고
	//UGameplayStatics::OpenLevel(GetInstance(), FName(*MapName), false);
	UWorld* World = GetWorld();
	if (World)
	{
		//맵 데이터 생성
		GenerateObstacleMapData(World);
		//파일로 저장
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

	// 2차원 배열 초기화
	ObstacleMaps.SetNum(MapHeight);
	for (int32 Y = 0; Y < MapHeight; ++Y)
	{
		ObstacleMaps[Y].SetNum(MapWidth);
		for (int32 X = 0; X < MapWidth; ++X)
		{

			FVector Start = FVector(X, Y, 2000); // 위에서
			FVector End = FVector(X, Y, 5);  // 아래로

			FHitResult HitResult;

			bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel6); // 6이 Obstacle 채널
			//DrawDebugLine(World, Start, End, bHit ? FColor::Green : FColor::Red, true);
			// 장애물 있는 곳 1로
			if (bHit)
			{
				/*FString ActorName = HitResult.GetActor()->GetName();
				UE_LOG(LogTemp, Warning, TEXT("Hit : %d %d %s"), X, Y, *ActorName);*/
				//GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Obstacle"));
				ObstacleMaps[Y][X] = 1;
			}
			else
			{
				//DrawDebugLine(World, Start, End, FColor::Red, true);
				//GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("No Obstacle"));
				ObstacleMaps[Y][X] = 0;
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