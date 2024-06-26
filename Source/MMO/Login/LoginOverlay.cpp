// Fill out your copyright notice in the Description page of Project Settings.


#include "Login/LoginOverlay.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"
#include "Misc/CString.h"
#include "PacketMaker/LoginPacketMaker.h"
#include "DataStructure/SerializeBuffer.h"
#include "GameInstance/MMOGameInstance.h"


void ULoginOverlay::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Warning, TEXT("NativeConstruct"))

        if (LoginButton != nullptr)
        {
            LoginButton->OnClicked.AddDynamic(this, &ULoginOverlay::OnLoginButtonClicked);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("LoginButton is nullptr"))
        }
}

FReply ULoginOverlay::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        // Enter 키가 눌렸을 때의 처리
        OnLoginButtonClicked(); // 예시로 버튼 클릭 함수 호출
        return FReply::Handled();
    }

    return FReply::Unhandled();
}



void ULoginOverlay::OnLoginButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnLoginButtonClicked"))
    FString UserName = UserNameTextBox->GetText().ToString();
    FString Password = PasswordTextBox->GetText().ToString();

    // 서버로 UserName과 Password를 전송하는 로직
    bool bRequestLoginSuccess = CallServerLoginFunction(UserName, Password);
    if (!bRequestLoginSuccess)
    {
        //TODO: 실패창 띄우기
        UE_LOG(LogTemp, Warning, TEXT("Connect Login Failed"));
    }
    else
    {
		UE_LOG(LogTemp, Warning, TEXT("Connect Login Success"));
    }

}

void ULoginOverlay::OnLoginSuccess()
{
    UE_LOG(LogTemp, Warning, TEXT("OnLoginSuccess"))
    UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/DefaultMap"), TRAVEL_Absolute);

    //TODO: LoginServer 연결도 끊어야함
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
    FCString::Strncpy(id, *UserName, 20);
    FCString::Strncpy(passWord, *Password, 20);

    CPacket* ReqLoginPacket = CPacket::Alloc();
    LoginPacketMaker::MP_CS_REQ_LOGIN(ReqLoginPacket, id, passWord);
    CPacket* EchoStartPacket = CPacket::Alloc();
    LoginPacketMaker::MP_CS_REQ_ECHO(EchoStartPacket);
    
    if (auto GameInstacne = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
    {
        bool connectSuccess = GameInstacne->ConnectLoginServer();
        if (!connectSuccess)
        {
            return false;
        }
        GameInstacne->SendPacket_LoginServer(ReqLoginPacket);
        GameInstacne->SendPacket_LoginServer(EchoStartPacket);
    }
    else {
        return false;
    }

    /*
    strncpy() 함수는 string2의 count자를 string1에 복사합니다.
    count가 string2 길이 이하이면 널(null) 문자(\0)는 복사된 
    스트링에 추가되지 않습니다. count가 string2의 길이보다 
    큰 경우 string1 결과는 길이 count까지 널(null) 문자(\0)로 채워집니다
    */

    // 실제 서버 로그인 처리 로직을 여기에 구현
    // 예제에서는 단순히 성공을 반환
    return true;
}