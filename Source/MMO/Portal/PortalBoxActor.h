// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalWidget.h"
#include "PortalBoxActor.generated.h"

class UBoxComponent;
class UParticleSystemComponent;
class UWidgetComponent;


UCLASS()
class MMO_API APortalBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalBoxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//� ��Ż
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	int32 FieldId;

	//�������Ʈ���� ���������ϱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	UParticleSystem* ParticleSystem;

	UPROPERTY()
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* PortalWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UPortalWidget> WidgetClass;


private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;



};
