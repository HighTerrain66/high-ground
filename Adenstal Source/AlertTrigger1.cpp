// Fill out your copyright notice in the Description page of Project Settings.


#include "AlertTrigger1.h"
#include "Components/AudioComponent.h"

AAlertTrigger1::AAlertTrigger1() 
{
    OnActorBeginOverlap.AddUniqueDynamic(this, &AAlertTrigger1::OverlapBegin);
}

void AAlertTrigger1::OverlapBegin(AActor* OverlappedActor, AActor* OtherActor) 
{
    //Super::OnOverlapBegin(OverlappedActor, OtherActor);

    

    if (!Activated && OtherActor == ActorThatOpens)
    {
        /* if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("ALERT"));
        } */
        if (AudioComponent)
        {
            AudioComponent->Stop();
            AudioComponent->Sound = AlertMusic;
            AudioComponent->Play();
        }
        Activated = true;
    }
}

void AAlertTrigger1::BeginPlay() 
{
    Super::BeginPlay();
    
}
