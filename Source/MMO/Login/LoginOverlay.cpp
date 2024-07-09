// Fill out your copyright notice in the Description page of Project Settings.


#include "Login/LoginOverlay.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"
#include "Misc/CString.h"
#include "DataStructure/SerializeBuffer.h"
#include "GameInstance/MMOGameInstance.h"
#include "PacketMaker/GamePacketMaker.h"
#include "SignUpOverlay.h"


void ULoginOverlay::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Warning, TEXT("NativeConstruct"))

    if (LoginButton != nullptr)
    {
        LoginButton->OnClicked.AddDynamic(this, &ULoginOverlay::OnLoginButtonClicked);
    }

    if (SignUpButton != nullptr)
	{
		SignUpButton->OnClicked.AddDynamic(this, &ULoginOverlay::OnSignUpButtonClicked);
	}
	
}

FReply ULoginOverlay::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        // Enter Ű�� ������ ���� ó��
        OnLoginButtonClicked(); // ���÷� ��ư Ŭ�� �Լ� ȣ��
        return FReply::Handled();
    }

    return FReply::Unhandled();
}



void ULoginOverlay::OnLoginButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnLoginButtonClicked"))
    FString UserName = UserNameTextBox->GetText().ToString();
    FString Password = PasswordTextBox->GetText().ToString();

    // ������ UserName�� Password�� �����ϴ� ����
    bool bRequestLoginSuccess = CallServerLoginFunction(UserName, Password);
    if (!bRequestLoginSuccess)
    {
        //TODO: ����â ����
        UE_LOG(LogTemp, Warning, TEXT("Connect Login Failed"));
    }
    else
    {
		UE_LOG(LogTemp, Warning, TEXT("Connect Login Success"));
    }

}

void ULoginOverlay::OnSignUpButtonClicked()
{
    if (SignUpWidgetClass)
    {
        SignUpOverlay = CreateWidget<USignUpOverlay>(GetWorld(), SignUpWidgetClass);
        if (SignUpOverlay)
        {
            SignUpOverlay->AddToViewport();
        }
    }
}

void ULoginOverlay::AddFailWidgetToViewport()
{
    if (FailWidgetClass)
	{
		FailWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), FailWidgetClass);
		if (FailWidgetInstance)
		{
			FailWidgetInstance->AddToViewport();
            //TODO: FailWidget�� �ִ� OkButton�� ���� �̺�Ʈ ���ε�
            UButton* OkButton = Cast<UButton>(FailWidgetInstance->GetWidgetFromName(TEXT("OkButton")));
            if (OkButton)
            {
                OkButton->OnClicked.AddDynamic(this, &ULoginOverlay::OnFailWidgetOkButtonClicked);
            }
		}
	}
}

void ULoginOverlay::OnFailWidgetOkButtonClicked()
{
    if (FailWidgetInstance)
	{
		FailWidgetInstance->RemoveFromParent();
		FailWidgetInstance = nullptr;
	}
}

void ULoginOverlay::OnLoginSuccess()
{
    UE_LOG(LogTemp, Warning, TEXT("OnLoginSuccess"))
    UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/DefaultMap"), TRAVEL_Absolute);

    //TODO: LoginServer ���ᵵ �������
}

bool ULoginOverlay::CallServerLoginFunction(const FString& UserName, const FString& Password)
{
    if (UserName.Len() > 19)
    {
        return false;
    }

    if (Password.Len() > 19)
    {
        return false;
    }

    TCHAR id[20];
    TCHAR passWord[20];
    FCString::Strncpy(id, *UserName, ID_LEN);
    FCString::Strncpy(passWord, *Password, PASSWORD_LEN);

    
    if (auto GameInstacne = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
    {
        CPacket* ReqLoginPacket = CPacket::Alloc();
        GamePacketMaker::MP_CS_REQ_LOGIN(ReqLoginPacket, id, passWord);
        GameInstacne->SendPacket_GameServer(ReqLoginPacket);
    }
    else {
        return false;
    }

    /*
    strncpy() �Լ��� string2�� count�ڸ� string1�� �����մϴ�.
    count�� string2 ���� �����̸� ��(null) ����(\0)�� ����� 
    ��Ʈ���� �߰����� �ʽ��ϴ�. count�� string2�� ���̺��� 
    ū ��� string1 ����� ���� count���� ��(null) ����(\0)�� ä�����ϴ�
    */

    // ���� ���� �α��� ó�� ������ ���⿡ ����
    // ���������� �ܼ��� ������ ��ȯ
    return true;
}