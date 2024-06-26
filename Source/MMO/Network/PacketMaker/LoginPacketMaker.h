// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CPacket;

class MMO_API LoginPacketMaker
{
public:
	static void MP_CS_REQ_LOGIN(CPacket* Packet, TCHAR* Id, TCHAR* PassWord);
	static void MP_CS_REQ_ECHO(CPacket* Packet);

private:
	inline static uint16 LoginPacketCode = 0x77;
};
