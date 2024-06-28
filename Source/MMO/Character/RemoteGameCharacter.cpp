// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RemoteGameCharacter.h"
#include "MMOComponent/CharAttributeComponent.h"
#include "HUD/HUDRemoteCharacterComponent.h"


ARemoteGameCharacter::ARemoteGameCharacter()
{
	CharAttributeComponent = CreateDefaultSubobject<UCharAttributeComponent>(TEXT("CharAttributeComponent"));
	//���⼭ ���� �ʱ�ȭ
	

	HUDRemoteCharacterComponent = CreateDefaultSubobject<UHUDRemoteCharacterComponent>(TEXT("HUDRemoteCharacterComponent"));
	HUDRemoteCharacterComponent->SetupAttachment(RootComponent);
}

void ARemoteGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	//�̷��ڵ�//
	if (HUDRemoteCharacterComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Set Monster HUD Component."));
		HUDRemoteCharacterComponent->SetCharacterName(CharAttributeComponent->GetCharName());
		HUDRemoteCharacterComponent->SetHealthPercent(CharAttributeComponent->GetHelathPercent());
		HUDRemoteCharacterComponent->SetCharacterLevel(CharAttributeComponent->GetLevel());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Monster HUD Component is nullptr."));
	}
}

void ARemoteGameCharacter::GetHit(int32 damage)
{
	CharAttributeComponent->GetDamage(damage);
	//CharAttributeComponent->GetDamage(damage);

	//���⼭ ���� HUD�����ؼ� SetHealthBarPercent�� ȣ���ؾ���
	
	//if (MMOOverlay && CharAttributeComponent)
	//{
	//	MMOOverlay->SetHealthBarPercent(CharAttributeComponent->GetHelathPercent());
	//}

	/*HUDCharacterComponent->SetHealthPercent(MonsAttributeComponent->GetHelathPercent());
	*/
	if (!CharAttributeComponent->IsAlive())
	{
		Death();
	}
}