// Fill out your copyright notice in the Description page of Project Settings.


#include "HintTrigger.h"
#include "ThiefCharacter.h"

void AHintTrigger::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AHintTrigger::OnOverlapBegin);
}

void AHintTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    AThiefCharacter* Character = Cast<AThiefCharacter>(OtherActor);
    UE_LOG(LogTemp, Error, TEXT("HINT"));
    if (Character == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("NULL CHARACTER"));
        return;
    }
    

    Character->OnReceiveHint(Hint);
    
    Destroy();
}
