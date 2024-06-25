// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/RecvThread.h"
#include "Sockets.h"
#include "PacketHeader.h"
#include "Network/ClientSession.h"
#include "GenericPlatform/GenericPlatformProcess.h"

RecvThread::RecvThread(ClientSession* s, FSocket* socket)
{
	Socket = socket;
	Session = s;
	Thread = FRunnableThread::Create(this, TEXT("RecvThread"));
}

RecvThread::~RecvThread()
{

}

bool RecvThread::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("RecvThread::Init()")));
	return true;
}

uint32 RecvThread::Run()
{
	while (!bShutdown)
	{
		int directEnqueueSize = recvBuffer.GetDirectEnqueueSize();
		if (directEnqueueSize == 0)
		{
			// 이경우 생기면 에러고
			UE_LOG(LogTemp, Error, TEXT("directEnqueueSize == 0"));
		}

		char* rearPtr = recvBuffer.GetRearPtr();
		int recvVal = 0;
		bool succeed = Socket->Recv((uint8*)rearPtr, directEnqueueSize, recvVal);

		if (!succeed)
		{
			// 게임종료하고 로그인씬 가는게 맞는데
			UE_LOG(LogTemp, Error, TEXT("RecvThread::Run() !succeed"));
		} else {

			while (true)
			{
				if (recvBuffer.GetUseSize() < sizeof(NetHeader))
				{
					break;
				}

				NetHeader header;
				recvBuffer.Peek((char*)&header, sizeof(NetHeader));
			
				int dataSize = header._len;
				if (recvBuffer.GetUseSize() < sizeof(NetHeader) +dataSize)
				{
					break;
				}

				recvBuffer.MoveFront(sizeof(NetHeader));

				//packet을 만들어서
				CPacket* packet = CPacket::Alloc();
				char* buf = packet->GetBufferPtr();
				//datasize만큼 긁어오고
				recvBuffer.Dequeue(buf, dataSize);
				packet->MoveWritePos(dataSize);

				bool bDecoedSucceed = packet->Decode(&header, 77);
				if (!bDecoedSucceed)
				{
					UE_LOG(LogTemp, Error, TEXT("RecvThread::Run() !bDecoedSucceed"));
				}

				Session->RecvPacketQueue.Enqueue(packet);
			}
		}
	}
	return 0;
}

void RecvThread::Exit()
{
}

void RecvThread::Shutdown()
{
	bShutdown  = true;
}
