// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


UCLASS()
class MMO_API AWeapon : public AItem
{
	GENERATED_BODY()


public:
	AWeapon();

protected:
	virtual void BeginPlay() override;


};
