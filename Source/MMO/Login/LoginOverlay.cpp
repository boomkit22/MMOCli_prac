// Fill out your copyright notice in the Description page of Project Settings.


#include "Login/LoginOverlay.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

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

bool ULoginOverlay::CallServerLoginFunction(const FString& UserName, const FString& Password)
{
    // 실제 서버 로그인 처리 로직을 여기에 구현
// 예제에서는 단순히 성공을 반환
    return true;
}

void ULoginOverlay::OnLoginButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnLoginButtonClicked"))
    FString UserName = UserNameTextBox->GetText().ToString();
    FString Password = PasswordTextBox->GetText().ToString();

    // 서버로 UserName과 Password를 전송하는 로직
    bool bLoginSuccess = CallServerLoginFunction(UserName, Password);

    if (bLoginSuccess)
    {
        // 로그인 성공 시 이벤트 호출
        // 예시로 버튼 클릭 이벤트 호출
        OnLoginSuccess();
    }
    else
    {
        // 로그인 실패 시 이벤트 호출
        // 예시로 버튼 클릭 이벤트 호출
        // OnLoginFailed();
    }
}

void ULoginOverlay::OnLoginSuccess()
{
    UE_LOG(LogTemp, Warning, TEXT("OnLoginSuccess"))
    UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/DefaultMap"), TRAVEL_Absolute);
}
