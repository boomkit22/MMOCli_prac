// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerThread/SendThread.h"
#include "Sockets.h"
#include "DataStructure/NetworkRingBuffer.h"
#include "ClientSession/ClientSession.h"

SendThread::SendThread(TSharedPtr<ClientSession> clientSession) : Session(clientSession)
{
	Socket = clientSession->Socket;
	SendBuffer = &(clientSession->SendBuffer);
	Thread = FRunnableThread::Create(this, TEXT("SendThread"));
}

SendThread::~SendThread()
{

}

bool SendThread::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SendThread::Init()")));
	return false;
}

uint32 SendThread::Run()
{
	while (!bShutdown)
	{
		int directDequeueSize = SendBuffer->GetDirectDequeueSize();
		if (directDequeueSize == 0)
		{
			continue;
		}

		char* fronPtr = SendBuffer->GetFrontPtr();
		int byteSent = 0;
		bool succeed = false;
		if (Socket)
		{
			UE_LOG(LogTemp, Warning, TEXT("Send"));
			succeed = Socket->Send((uint8*)fronPtr, directDequeueSize, byteSent);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Socket = nullptr"));
			if (TSharedPtr<ClientSession> session = Session.Pin())
			{
				StopThread();
				session->NetworkDisconnect();
				break;
			}
		}

		if (!succeed)
		{
			//Send �����ϸ� �������������ʳ�
			UE_LOG(LogTemp, Error, TEXT("Network Disconnect"));

			if (TSharedPtr<ClientSession> session = Session.Pin())
			{
				StopThread();
				session->NetworkDisconnect();
				break;
			}

		}


		if (directDequeueSize != byteSent)
		{
			//�̰͵� ���� ������ϰ�
			UE_LOG(LogTemp, Error, TEXT("SendThread::Run() !succeed"));
		}
		SendBuffer->MoveFront(byteSent);
	}

	return 0;
}

void SendThread::Exit()
{
}

void SendThread::StopThread()
{
	// close socket ���� ����߰ڴµ�
	bShutdown = true;
	Thread->WaitForCompletion();
}
