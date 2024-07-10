// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMO_API UCharAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init(int32 Health, FString CharName, int32 Level);
	FORCEINLINE FString GetCharName() { return CharName; };
	FORCEINLINE int32 GetLevel() { return Level; };


private:
	//Current Health
	UPROPERTY(EditAnywhere, Category = "CharAttribute")
	int32 Health;

	UPROPERTY(EditAnywhere, Category = "CharAttribute")
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, Category = "CharAttribute")
	FString CharName;

	UPROPERTY(EditAnywhere, Category = "CharAttribute")
	uint16 Level;

	UPROPERTY(EditAnywhere, Category = "CharAttribute")
	uint32 Exp;
	
	int64 PlayerID;

public:
	bool IsAlive();
	float GetHelathPercent();
	void GetDamage(int damage);
	void RecoverHealth(int recover);

};
