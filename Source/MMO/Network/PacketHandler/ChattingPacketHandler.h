// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstance/MMOGameInstance.h"

/**
 * 
 */
class CPacket;

class MMO_API ChattingPacketHandler
{
public:
	static void HandlePacket(CPacket* packet);

private:
	static void HandleLogin(CPacket* packet);
	static void HandleMessage(CPacket* packet);

private:
	inline static UMMOGameInstance* GameInstance = nullptr;
};
