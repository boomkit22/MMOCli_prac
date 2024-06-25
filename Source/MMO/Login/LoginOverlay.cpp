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
        // Enter Ű�� ������ ���� ó��
        OnLoginButtonClicked(); // ���÷� ��ư Ŭ�� �Լ� ȣ��
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

bool ULoginOverlay::CallServerLoginFunction(const FString& UserName, const FString& Password)
{
    // ���� ���� �α��� ó�� ������ ���⿡ ����
// ���������� �ܼ��� ������ ��ȯ
    return true;
}

void ULoginOverlay::OnLoginButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnLoginButtonClicked"))
    FString UserName = UserNameTextBox->GetText().ToString();
    FString Password = PasswordTextBox->GetText().ToString();

    // ������ UserName�� Password�� �����ϴ� ����
    bool bLoginSuccess = CallServerLoginFunction(UserName, Password);

    if (bLoginSuccess)
    {
        // �α��� ���� �� �̺�Ʈ ȣ��
        // ���÷� ��ư Ŭ�� �̺�Ʈ ȣ��
        OnLoginSuccess();
    }
    else
    {
        // �α��� ���� �� �̺�Ʈ ȣ��
        // ���÷� ��ư Ŭ�� �̺�Ʈ ȣ��
        // OnLoginFailed();
    }
}

void ULoginOverlay::OnLoginSuccess()
{
    UE_LOG(LogTemp, Warning, TEXT("OnLoginSuccess"))
    UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/DefaultMap"), TRAVEL_Absolute);
}
