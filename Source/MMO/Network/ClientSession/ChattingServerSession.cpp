// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSession/ChattingServerSession.h"
#include "PacketHandler/ChattingPacketHandler.h"
#include "GameInstance/MMOGameInstance.h"

ChattingServerSession::ChattingServerSession()
{
}

ChattingServerSession::~ChattingServerSession()
{
}

void ChattingServerSession::HandleRecvPacket()
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

		ChattingPacketHandler::HandlePacket(packet);
		/*LoginPacketHandler::HandlePacket(packet);*/
	}
}

void ChattingServerSession::OnDisconnect()
{
}
