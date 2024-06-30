// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMOChatOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UMMOChatOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void NativeConstruct() override;

    UFUNCTION()
    void OnSendButtonClicked();

private:
    UPROPERTY(meta = (BindWidget))
    class UScrollBox* ChatScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* ChatVerticalBox;

    UPROPERTY(meta = (BindWidget))
    class UEditableText* MessageEditableText;

    UPROPERTY(meta = (BindWidget))
    class UButton* SendButton;
};
