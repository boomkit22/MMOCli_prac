// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSession/GameServerSession.h"
#include "PacketHandler/GamePacketHandler.h"
#include "GameInstance/MMOGameInstance.h"
GameServerSession::GameServerSession()
{
}

GameServerSession::~GameServerSession()
{
}

void GameServerSession::HandleRecvPacket()
{
	for (;;)
	{
		if (bLoading)
		{
			break;
		}

		CPacket* packet = nullptr;
		bool Succeed = RecvPacketQueue.Dequeue(packet);
		if (!Succeed)
		{
			break;
		}

		GamePacketHandler::HandlePacket(packet);
		/*LoginPacketHandler::HandlePacket(packet);*/
	}
}

void GameServerSession::OnDisconnect()
{
}
