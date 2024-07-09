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
	// ȸ������ ��ư�� Ŭ���Ǿ��� ���� ������ ���⿡ �����մϴ�.
	FString Username = UsernameTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();

	//TODO: ���Ӽ����� ���

	
	//������
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
	// �ڷΰ��� ��ư�� Ŭ���Ǿ��� ���� ������ ���⿡ �����մϴ�.
	RemoveFromParent();
}

void USignUpOverlay::OnSuccessWidgetOkButtonClicked()
{
	if (SuccessWidgetInstance)
	{
		SuccessWidgetInstance->RemoveFromParent();
		SuccessWidgetInstance = nullptr;
	}
	RemoveFromParent(); // SignUpWidget ����
}

void USignUpOverlay::OnFailWidgetOkButtonClicked()
{
	if (FailWidgetInstance)
	{
		FailWidgetInstance->RemoveFromParent();
		FailWidgetInstance = nullptr;
	}
}
