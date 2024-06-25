// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientSession.h"


/**
 * 
 */
class MMO_API ChattingServerSession : public ClientSession
{
public:
	ChattingServerSession();
	~ChattingServerSession();

	void HandleRecvPacket() override;
};
