// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoginHUD.generated.h"

/**
 * 
 */

class ULoginOverlay;
class UCharacterSelectOverlay;
UCLASS()
class MMO_API ALoginHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void ChangeOverlay(TSubclassOf<UUserWidget> NewOverlayClass);
	TSubclassOf<ULoginOverlay> GetLoginOverlayClass() const { return LoginOverlayClass; };
	TSubclassOf<UCharacterSelectOverlay> GetCharacterSelectOverlayClass() const { return CharacterSelectOverlayClass; };


protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditDefaultsOnly, Category = "MMO")
	TSubclassOf<ULoginOverlay> LoginOverlayClass;
	UPROPERTY(EditDefaultsOnly, Category = "MMO")
	TSubclassOf<UCharacterSelectOverlay> CharacterSelectOverlayClass;
	
	//UPROPERTY()
	//ULoginOverlay* LoginOverlay;
	//UPROPERTY()
	//UCharacterSelectOverlay* CharacterSelectOverlay;

	UUserWidget* CurrentOverlay;


public:
	/*FORCEINLINE ULoginOverlay* GetLoginOverlay() const { return LoginOverlay; }
	FORCEINLINE UCharacterSelectOverlay* GetCharacterSelectOverlay() const { return CharacterSelectOverlay; }*/
};
