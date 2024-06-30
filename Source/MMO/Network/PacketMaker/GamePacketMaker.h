// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Type.h"
/**
 * 
 */
class CPacket;

class MMO_API GamePacketMaker
{
public:
	static void MP_CS_REQ_LOGIN(CPacket* Packet, int64& accountNo);
	static void MP_CS_REQ_FIELD_MOVE(CPacket* Packet, uint16& fieldID);
	static void MP_CS_REQ_CHARACTER_MOVE(CPacket* Packet, FVector& Destination, FRotator& StartRotation);
	static void MP_CS_REQ_CHARACTER_ATTACK(CPacket* packet, AttackInfo& attackInfo);
	static void MP_SC_REQ_CHARACTER_SKILL(CPacket* packet, FRotator& StartRotator, int32 SkillID);

private:
	inline static uint16 GamePacketCode = 0x77;

};
