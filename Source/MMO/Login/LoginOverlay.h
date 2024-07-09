// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API ULoginOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	bool CallServerLoginFunction(const FString& UserName, const FString& Password);

	void AddFailWidgetToViewport();

	class USignUpOverlay* GetSignUpOverlay() const { return SignUpOverlay; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* UserNameTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* PasswordTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* LoginButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SignUpButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> SignUpWidgetClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> FailWidgetClass;

	UPROPERTY()
	class UUserWidget* FailWidgetInstance;


protected:
	// bind하는건 UFUNCTION안하면 에러남
	UFUNCTION()
	void OnLoginButtonClicked();

	UFUNCTION()
	void OnSignUpButtonClicked();


	UFUNCTION()
	void OnFailWidgetOkButtonClicked();

private:
	void OnLoginSuccess();
	class USignUpOverlay* SignUpOverlay;
};
