// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkGameInstanceSubsystem.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Engine/Engine.h"

void UNetworkGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker
	(FTickerDelegate::CreateUObject(this, &UNetworkGameInstanceSubsystem::Tick));
}

void UNetworkGameInstanceSubsystem::Connect()
{
	//Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Socket"));
	//FIPv4Address Ip;
	//FIPv4Address::Parse(IpText, Ip);

	//TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	//InternetAddr->SetIp(Ip.Value);
	//InternetAddr->SetPort(Port);

	//Connected = Socket->Connect(*InternetAddr);

	//if (Connected)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

	//	// Session
	//	Session = new ClientSession(Socket);
	//	Session->Init();
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	//}
}

void UNetworkGameInstanceSubsystem::SendPacket(CPacket* packet)
{
	if (Connected)
	{
		int enqueueVal = Session->SendBuffer.Enqueue(packet->GetBufferPtr(), packet->GetDataSize());
		if (enqueueVal == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("enqueueVal ==0"));
		}
	}
}

void UNetworkGameInstanceSubsystem::ProcessRecvPcket()
{
	if (Session == nullptr || Socket == nullptr)
	{
		return;
	}

	Session->HandleRecvPacket();
}

void UNetworkGameInstanceSubsystem::Handle_CREATE_MY_CHARACTER(uint32 id, FVector3f& position, float yaw, uint16 hp)
{
	UE_LOG(LogTemp, Warning, TEXT("Handle_CREATE_MY_CHARACTER"));
}

bool UNetworkGameInstanceSubsystem::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("UNetworkGameInstanceSubsystem::Tick"));

	if (Connected)
	{
		ProcessRecvPcket();
	}
	
	return true; // return true to keep the ticker running, false to stop
}
