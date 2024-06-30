// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#define ID_LEN 20
#define NICKNAME_LEN 20
#define PASSWORD_LEN 20
#define TYPE_PLAYER  1
#define TYPE_MONSTER  2
class CPacket;


CPacket& operator<<(CPacket& packet, FRotator& rot);
CPacket& operator>>(CPacket& packet, FRotator& rot);

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


struct AttackInfo
{
	int32 AttackerType;
	int64 AttackerID;
	int32 TargetType;
	int64 TargetID;
	int32 Damage;
};

CPacket& operator<<(CPacket& packet, AttackInfo& attackInfo);
CPacket& operator>>(CPacket& packet, AttackInfo& attackInfo);







