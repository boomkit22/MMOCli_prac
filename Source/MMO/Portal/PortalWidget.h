// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortalWidget.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UPortalWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    void SetPortalText(const FString& NewText);

protected:
    virtual void NativeConstruct() override;

    // 위젯의 텍스트 블록을 바인딩
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* NameText;
};
