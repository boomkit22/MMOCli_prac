// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MMOChatOverlay.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMMOChatOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	if (SendButton)
	{
		SendButton->OnClicked.AddDynamic(this, &UMMOChatOverlay::OnSendButtonClicked);
	}
}

void UMMOChatOverlay::OnSendButtonClicked()
{
    if (MessageEditableText && ChatVerticalBox)
    {
        FString InputText = MessageEditableText->GetText().ToString();
        if (!InputText.IsEmpty())
        {
            UTextBlock* NewMessage = NewObject<UTextBlock>(ChatVerticalBox);
            if (NewMessage)
            {
                NewMessage->SetText(FText::FromString(InputText));
                ChatVerticalBox->InsertChildAt(0, NewMessage);

                // Clear the input box
                MessageEditableText->SetText(FText::GetEmpty());

                // Scroll to the bottom
                ChatScrollBox->ScrollToEnd();
            }
        }
    }
}
