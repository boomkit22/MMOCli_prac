// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RemoteGameCharacter.h"
#include "MMOComponent/CharAttributeComponent.h"
#include "HUD/HUDRemoteCharacterComponent.h"
#include "Type.h"


ARemoteGameCharacter::ARemoteGameCharacter()
{
	CharAttributeComponent = CreateDefaultSubobject<UCharAttributeComponent>(TEXT("CharAttributeComponent"));
	//여기서 위젯 초기화
	

	HUDRemoteCharacterComponent = CreateDefaultSubobject<UHUDRemoteCharacterComponent>(TEXT("HUDRemoteCharacterComponent"));
	HUDRemoteCharacterComponent->SetupAttachment(RootComponent);
}

void ARemoteGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARemoteGameCharacter::InitCharAttributeComponent(int32 Health, FString CharName, int32 Level)
{
	CharAttributeComponent->Init(Health, CharName, Level);
	if (HUDRemoteCharacterComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Set Monster HUD Component."));
		HUDRemoteCharacterComponent->SetCharacterName(CharAttributeComponent->GetCharName());
		HUDRemoteCharacterComponent->SetHealthPercent(CharAttributeComponent->GetHelathPercent());
		HUDRemoteCharacterComponent->SetCharacterLevel(CharAttributeComponent->GetLevel());
	}
}

void ARemoteGameCharacter::GetHit(int32 damage)
{
	//CharAttributeComponent->GetDamage(damage);

	//여기서 이제 HUD접근해서 SetHealthBarPercent를 호출해야함
	
	//if (MMOOverlay && CharAttributeComponent)
	//{
	//	MMOOverlay->SetHealthBarPercent(CharAttributeComponent->GetHelathPercent());
	//}

	/*HUDCharacterComponent->SetHealthPercent(MonsAttributeComponent->GetHelathPercent());
	*/
	CharAttributeComponent->GetDamage(damage);
	HUDRemoteCharacterComponent->SetHealthPercent(CharAttributeComponent->GetHelathPercent());

	if (!CharAttributeComponent->IsAlive())
	{
		Death();
	}
}

int ARemoteGameCharacter::GetType()
{
	return TYPE_PLAYER;
}

int64 ARemoteGameCharacter::GetId()
{
	return PlayerID;
}

