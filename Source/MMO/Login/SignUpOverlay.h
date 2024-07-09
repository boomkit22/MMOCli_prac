// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignUpOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API USignUpOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* UsernameTextBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* SignUpButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UFUNCTION()
	void OnSignUpButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> SuccessWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> FailWidgetClass;

	UFUNCTION()
	void OnSuccessWidgetOkButtonClicked();

	UFUNCTION()
	void OnFailWidgetOkButtonClicked();

	UPROPERTY()
	class UUserWidget* SuccessWidgetInstance;

	UPROPERTY()
	class UUserWidget* FailWidgetInstance;

};
