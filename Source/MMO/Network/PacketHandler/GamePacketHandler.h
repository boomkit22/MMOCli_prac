// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CPacket;
class MMO_API GamePacketHandler
{
public:
	static void HandlePacket(CPacket* packet);

private:
	static void HandleLogin(CPacket* packet);
	static void HandleFieldMove(CPacket* packet);
	static void HandleSpawnMyCharacter(CPacket* packet);
	static void HandleSpawnOtherCharacter(CPacket* packet);
	static void HandleCharacterMove(CPacket* packet);
	static void HandleDamage(CPacket* packet);
};
