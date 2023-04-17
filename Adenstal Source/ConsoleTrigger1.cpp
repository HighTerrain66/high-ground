// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleTrigger1.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "ShooterCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterAIController.h"

void AConsoleTrigger1::BeginPlay() 
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = false;
    SetActorTickEnabled(false);
    AudioComponent = FindComponentByClass<UAudioComponent>();
    UE_LOG(LogTemp, Warning, TEXT("YESYESY"));
    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
    UE_LOG(LogTemp, Warning, TEXT("%s"), *GetWorld()->GetFirstPlayerController()->GetPawn()->GetName());
    
}

AConsoleTrigger1::AConsoleTrigger1() 
{
    OnActorBeginOverlap.AddDynamic(this, &AConsoleTrigger1::OnOverlapBegin);
}

void AConsoleTrigger1::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);
    UE_LOG(LogTemp, Warning, TEXT("OVERLAPtick"));
    if (!Activated && ActorThatOpens)
    {
        UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN YES"));
        
    }
    
}

void AConsoleTrigger1::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) 
{
    UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN YES GREAT"));
    /* if (!Activated && OtherActor == ActorThatOpens)
    {
        

        Activated = true;
    } */
    
}

