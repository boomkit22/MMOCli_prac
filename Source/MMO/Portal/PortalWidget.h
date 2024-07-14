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

    // ������ �ؽ�Ʈ ����� ���ε�
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* NameText;
};
