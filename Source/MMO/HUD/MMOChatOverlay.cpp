// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MMOChatOverlay.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "PacketMaker/ChattingPacketMaker.h"
#include "GameInstance/MMOGameInstance.h"
void UMMOChatOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	if (SendButton)
	{
		SendButton->OnClicked.AddDynamic(this, &UMMOChatOverlay::OnSendButtonClicked);
	}

    AccountId = UMMOGameInstance::GetInstance()->GetAccountId();
}

void UMMOChatOverlay::OnSendButtonClicked()
{
    if (MessageEditableText && ChatVerticalBox)
    {
        FString InputText = MessageEditableText->GetText().ToString();
        if (!InputText.IsEmpty())
        {
            CPacket* messagePacket = CPacket::Alloc();
            ChattingPacketMaker::MP_CS_REQ_MESSAGE(messagePacket, AccountId, InputText);
            UMMOGameInstance::GetInstance()->SendPacket_ChattingServer(messagePacket);
            // Clear the input box
            MessageEditableText->SetText(FText::GetEmpty());
        }
    }
}

void UMMOChatOverlay::OnRecvMessage(const FString& Message)
{
    if (ChatVerticalBox)
	{
		UTextBlock* NewMessage = NewObject<UTextBlock>(ChatVerticalBox);
		if (NewMessage)
		{
			NewMessage->SetText(FText::FromString(Message));
			ChatVerticalBox->AddChildToVerticalBox(NewMessage);

			// Scroll to the bottom
			ChatScrollBox->ScrollToEnd();
		}
	}
}

