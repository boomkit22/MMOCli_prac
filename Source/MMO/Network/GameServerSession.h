// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientSession.h"

/**
 * 
 */
class MMO_API GameServerSession : public ClientSession
{
public:
	GameServerSession();
	~GameServerSession();

	void HandleRecvPacket() override;
};
