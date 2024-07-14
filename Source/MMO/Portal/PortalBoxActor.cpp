// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/PortalBoxActor.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameData.h"

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
    
    PortalWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PortalWidgetComponent"));
    PortalWidgetComponent->SetupAttachment(RootComponent);
    PortalWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f)); // �ݸ��� �ڽ� ���� ��ġ
    PortalWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    PortalWidgetComponent->SetDrawSize(FVector2D(500.0f, 300.0f)); // ���� ũ�� ����
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

    if (PortalWidgetComponent)
    {
        PortalWidgetComponent->SetWidgetClass(WidgetClass);
        PortalWidgetComponent->InitWidget();
    }

    UPortalWidget* Widget = Cast<UPortalWidget>(PortalWidgetComponent->GetUserWidgetObject());
    if (Widget)
    {
        switch (FieldId)
        {
            case FIELD_GUARDIAN:
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SET TEXT GUARDIAN")));
                Widget->SetPortalText(FString("GUARDIAN"));
            }
            break;

            case FIELD_SPIDER:
	    	{
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SET TEXT SPIDER")));
	    		Widget->SetPortalText(FString("SPIDER"));
	    	}
            break;
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SET TEXT NULL")));
    }

}

// Called every frame
void APortalBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

