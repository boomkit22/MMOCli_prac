// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/PortalBoxActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APortalBoxActor::APortalBoxActor()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionObjectType(ECC_GameTraceChannel5); // Och_Portal ObjectChannel ����
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); // Player�ϰ� Overlap 2�� Player




}

// Called when the game starts or when spawned
void APortalBoxActor::BeginPlay()
{
	Super::BeginPlay();

    if (ParticleSystem)
    {
        ParticleSystemComponent = NewObject<UParticleSystemComponent>(this);
        ParticleSystemComponent->SetupAttachment(RootComponent);
        ParticleSystemComponent->SetTemplate(ParticleSystem);
        ParticleSystemComponent->RegisterComponent();
    }

    if (MapInfoWidgetClass)
    {
        MapInfoWidgetComponent = NewObject<UWidgetComponent>(this);
        MapInfoWidgetComponent->SetupAttachment(RootComponent);
        MapInfoWidgetComponent->SetWidgetClass(MapInfoWidgetClass);
        MapInfoWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // �̰� �����ϰ� �����ϰ�
        MapInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
        MapInfoWidgetComponent->RegisterComponent();
    }

}

// Called every frame
void APortalBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

