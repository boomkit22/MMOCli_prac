// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOComponent/MonsAttributeComponent.h"

// Sets default values for this component's properties
UMonsAttributeComponent::UMonsAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMonsAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMonsAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMonsAttributeComponent::Init(int32 health, FString monsterName)
{
	this->Health = health;
	this->MaxHealth = health;
	this->MonsterName = monsterName;
}

bool UMonsAttributeComponent::IsAlive()
{
	if (Health > 0)
	{
		return true;
	}
	return false;
}

float UMonsAttributeComponent::GetHelathPercent()
{
	if (MaxHealth > 0)
	{
		return Health / (float)MaxHealth;
	}
	return 0.0f;
}

void UMonsAttributeComponent::GetDamage(int damage)
{
	Health -= damage;
	if (Health < 0)
	{
		Health = 0;
	}
}

