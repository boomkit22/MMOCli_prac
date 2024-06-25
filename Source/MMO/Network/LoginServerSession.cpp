// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/LoginServerSession.h"
#include "Network/LoginPacketHandler.h"
LoginServerSession::LoginServerSession()
{
}

LoginServerSession::~LoginServerSession()
{
}

void LoginServerSession::HandleRecvPacket()
{
	for (;;)
	{
		CPacket* packet = nullptr;
		bool bNotEmpty = RecvPacketQueue.Dequeue(packet);
		if (!bNotEmpty)
		{
			break;
		}
		
		LoginPacketHandler::HandlePacket(packet);
		/*LoginPacketHandler::HandlePacket(packet);*/
	}
}

