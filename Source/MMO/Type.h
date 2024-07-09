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

struct PlayerInfo
{
	int64 PlayerID;
	TCHAR NickName[NICKNAME_LEN];
	uint16 Class;
	uint16 Level;
	uint32 Exp;
};

CPacket& operator<<(CPacket& packet, PlayerInfo& info);
CPacket& operator>>(CPacket& packet, PlayerInfo& info);







