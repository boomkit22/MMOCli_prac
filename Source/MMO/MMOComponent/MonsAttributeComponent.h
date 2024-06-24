// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMO_API UMonsAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	//Current Health
	UPROPERTY(EditAnywhere, Category = "MonsAttribute")
	float Health;

	UPROPERTY(EditAnywhere, Category = "MonsAttribute")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "MonsAttribute")
	FString MonsterName;


		
};
