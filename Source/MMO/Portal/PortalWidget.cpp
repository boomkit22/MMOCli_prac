// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/PortalWidget.h"
#include "Components/TextBlock.h"

void UPortalWidget::SetPortalText(const FString& NewText)
{
	if (NameText)
	{
		NameText->SetText(FText::FromString(NewText));
	}
}

void UPortalWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
