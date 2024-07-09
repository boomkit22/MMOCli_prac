// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Type.h"
/**
 * 
 */

class CPacket;

class MMO_API ChattingPacketMaker
{

public:
	static void MP_CS_REQ_LOGIN(CPacket* Packet, int64& AccountNo, TCHAR* NickName);
	static void MP_CS_REQ_MESSAGE(CPacket* Packet, int64& AccountNo, FString& message);

private:
	inline static uint16 GamePacketCode = 0x77;

};
