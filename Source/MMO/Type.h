// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define ID_LEN 20
#define NICKNAME_LEN 20
#define PASSWORD_LEN 20

class CPacket;


CPacket& operator<<(CPacket& packet, FVector& vec);
CPacket& operator>>(CPacket& packet, FVector& vec);


struct ResGameLoginInfo
{
	int64 AccountNo;
	uint8 Status;
	uint16 Level;
	TCHAR NickName[ID_LEN];
};

CPacket& operator<<(CPacket& packet, ResGameLoginInfo& resLoginInfo);
CPacket& operator>>(CPacket& packet, ResGameLoginInfo& resLoginInfo);


struct SpawnMyCharacterInfo
{
	FVector SpawnLocation;
	int64 PlayerID;
	uint16 Level;
	TCHAR NickName[ID_LEN];
};

CPacket& operator<<(CPacket& packet, SpawnMyCharacterInfo& spawnMyCharacterInfo);
CPacket& operator>>(CPacket& packet, SpawnMyCharacterInfo& spawnMyCharacterInfo);


struct SpawnOtherCharacterInfo
{
	FVector SpawnLocation;
	int64 PlayerID;
	uint16 Level;
	TCHAR NickName[ID_LEN];
};

CPacket& operator<<(CPacket& packet, SpawnOtherCharacterInfo& spawnOtherCharacterInfo);
CPacket& operator>>(CPacket& packet, SpawnOtherCharacterInfo& spawnOtherCharacterInfo);









