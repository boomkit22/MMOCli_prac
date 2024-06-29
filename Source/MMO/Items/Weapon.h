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

	
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")

	class USkeletalMeshComponent* WeaponMesh;

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* WeaponBox;
public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

};
