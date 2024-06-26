// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSession/LoginServerSession.h"
#include "PacketHandler/LoginPacketHandler.h"

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
		bool Succeed = RecvPacketQueue.Dequeue(packet);
		if (!Succeed)
		{
			break;
		}
		
		LoginPacketHandler::HandlePacket(packet);
		/*LoginPacketHandler::HandlePacket(packet);*/
	}
}

void LoginServerSession::OnDisconnect()
{
}

