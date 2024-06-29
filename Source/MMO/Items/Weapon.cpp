// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());




}


//void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
//	bool bFromSweep, const FHitResult& 
//{
//	// 로그 메시지 출력
//	UE_LOG(LogTemp, Warning, TEXT("Weapon Box Overlap with: %s"), *OtherActor->GetName());
//	DrawDebugSphere(GetWorld(), OtherActor->GetActorLocation(), 10.0f, 12, FColor::Red, false, 1.0f);
//	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	//WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
}




