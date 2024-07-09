// Fill out your copyright notice in the Description page of Project Settings.


#include "Login/SignUpOverlay.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"


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

void USignUpOverlay::OnSignUpButtonClicked()
{
	// 회원가입 버튼이 클릭되었을 때의 동작을 여기에 정의합니다.
	FString Username = UsernameTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();

	//TODO: 게임서버로 쏘기

	
	//성공시
	if (SuccessWidgetClass)
	{
		SuccessWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), SuccessWidgetClass);
		if(SuccessWidgetInstance)
		{
			SuccessWidgetInstance->AddToViewport();

			UButton* OkButton = Cast<UButton>(SuccessWidgetInstance->GetWidgetFromName(TEXT("OkButton")));
			if (OkButton)
			{
				OkButton->OnClicked.AddDynamic(this, &USignUpOverlay::OnSuccessWidgetOkButtonClicked);
			}
		
		}

	
	}
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
