// Fill out your copyright notice in the Description page of Project Settings.


#include "Login/SignUpOverlay.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "Network/PacketMaker/GamePacketMaker.h"
#include "GameInstance/MMOGameInstance.h"


void USignUpOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	if (SignUpButton)
	{
		SignUpButton->OnClicked.AddDynamic(this, &USignUpOverlay::OnSignUpButtonClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USignUpOverlay::OnBackButtonClicked);
	}
}

void USignUpOverlay::AddSuccessWidgetToViewport()
{
	if (SuccessWidgetClass)
	{
		SuccessWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), SuccessWidgetClass);
		if (SuccessWidgetInstance)
		{
			SuccessWidgetInstance->AddToViewport();
			//TODO: FailWidget에 있는 OkButton에 대한 이벤트 바인딩
			UButton* OkButton = Cast<UButton>(SuccessWidgetInstance->GetWidgetFromName(TEXT("OkButton")));
			if (OkButton)
			{
				OkButton->OnClicked.AddDynamic(this, &USignUpOverlay::OnSuccessWidgetOkButtonClicked);
			}
		}
	}
}

void USignUpOverlay::AddFailWidgetToViewport()
{
	if (FailWidgetClass)
	{
		FailWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), FailWidgetClass);
		if (FailWidgetInstance)
		{
			FailWidgetInstance->AddToViewport();
			//TODO: FailWidget에 있는 OkButton에 대한 이벤트 바인딩
			UButton* OkButton = Cast<UButton>(FailWidgetInstance->GetWidgetFromName(TEXT("OkButton")));
			if (OkButton)
			{
				OkButton->OnClicked.AddDynamic(this, &USignUpOverlay::OnFailWidgetOkButtonClicked);
			}
		}
	}
}

void USignUpOverlay::OnSignUpButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnSignUpButtonClicked"));

	// 회원가입 버튼이 클릭되었을 때의 동작을 여기에 정의합니다.
	FString Username = UserNameTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();

	//여기다 Fail Widget하고 Message까지 같이 띄워도 될것같은데
	if (Username.Len() > 19)
	{
		return;
	}

	if (Password.Len() > 19)
	{
		return;
	}

	//TODO: 게임서버로 쏘기
	//------------------------------------------------------------
	//	{
	//		WORD	Type
	// 		TCHAR   ID[20]
	//		TCHAR	PassWord[20]     //사용자 PassWord. null포함
	//	}
	//------------------------------------------------------------
	CPacket* packet = CPacket::Alloc();
	TCHAR id[20];
	TCHAR passWord[20];
	FCString::Strncpy(id, *Username, ID_LEN);
	FCString::Strncpy(passWord, *Password, PASSWORD_LEN);

	GamePacketMaker::MP_CS_REQ_SIGN_UP(packet, id, passWord);
	UMMOGameInstance::GetInstance()->SendPacket_GameServer(packet);
}

void USignUpOverlay::OnBackButtonClicked()
{
	// 뒤로가기 버튼이 클릭되었을 때의 동작을 여기에 정의합니다.
	RemoveFromParent();
}

void USignUpOverlay::OnSuccessWidgetOkButtonClicked()
{
	if (SuccessWidgetInstance)
	{
		SuccessWidgetInstance->RemoveFromParent();
		SuccessWidgetInstance = nullptr;
	}
	RemoveFromParent(); // SignUpWidget 제거
}

void USignUpOverlay::OnFailWidgetOkButtonClicked()
{
	if (FailWidgetInstance)
	{
		FailWidgetInstance->RemoveFromParent();
		FailWidgetInstance = nullptr;
	}
}
