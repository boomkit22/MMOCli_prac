// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//#include "Network/GameSession.h"
#include "MMOGameInstance.generated.h"


/**
 * 
 */

class GameServerSession;
class LoginServerSession;
class ChattingServerSession;
class CPacket;
class AMyGameCharacter;

extern bool bLoading;

UCLASS()
class MMO_API UMMOGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnStart() override;

public:
	bool ConnectGameServer();
	bool ConnectChattingServer();

	void DisconnectGameServer();
	void DisconnectChattingServer();

public:
	void SendPacket_GameServer(CPacket* packet);
	void SendPacket_ChattingServer(CPacket* packet);
	void OpenLevel(FName LevelName);


/*
Game Handle
*/
public:
	void HandleGameLogin(CPacket* packet);
	void HandleFieldMove(CPacket* packet);
	void HandleSpawnMyCharacter(CPacket* packet);
	void HandleSpawnOhterCharacter(CPacket* packet);
	void HandleDespawnOtherCharacter(CPacket* packet);
	//void HandleCharacterMove(CPacket* packet);
	void HandleDamage(CPacket* packet);
	void HandleCharacterSkill(CPacket* packet);
    void HandleMonsterSkill(CPacket* packet);
	void HandleSignUp(CPacket* packet);
	void HandlePlayerList(CPacket* packet);
	void HandleCreatePlayer(CPacket* packet);
	void HandleSelectPlayer(CPacket* packet);
	void HandleSpawnMonster(CPacket* packet);
	void HandleMonsterMove(CPacket* packet);
	void HandleCharacterStop(CPacket* packet);
	void HandleMonsterStop(CPacket* packet);
	void HandleCharacterDeath(CPacket* packet);
	void HandleMonsterDeath(CPacket* packet);
	void HandleDespawnMonster(CPacket* packet);
	void HandleFindPath(CPacket* packet);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AActor> GameCharacterClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AActor> RemoteGameCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	TSubclassOf<AActor> MonsterClass;


/*
Chat Handle
*/
public:
	void HandleChatMessage(CPacket* packet);

private:
	TSharedPtr<GameServerSession> _GameServerSession;
	TSharedPtr<ChattingServerSession> _ChattingServerSession;

	/*GameServerSession* _GameServerSession = nullptr;
	LoginServerSession* _LoginServerSession = nullptr;
	ChattingServerSession* _ChattingServerSession = nullptr;*/

private:
	FTSTicker::FDelegateHandle TickDelegateHandle;
	bool Tick(float DeltaTime);

public:
	static UWorld* GetMMOWorld();
	static UMMOGameInstance* GetInstance();
	FORCEINLINE uint16 GetCurrentFieldID() { return CurrentFieldID; };
private:
	uint16 CurrentFieldID = 0;

public:
	FORCEINLINE int64 GetAccountId() { return AccountId; };

private:
	int64 AccountId;
	//bool bLoading = false;


	//FWorldDelegates::FWorldInitializationEvent::FDelegate OnPostWorldInitHandle;
	void OnLevelLoaded(UWorld* World);

private:
	UPROPERTY()
	TMap<int64, class AGameCharacter*> CharacterMap;
	UPROPERTY()
	TMap<int64, class AMonster*> MonsterMap;

	AMyGameCharacter* MyCharacter = nullptr;

	const double PlayerZValue = 95.2;
};