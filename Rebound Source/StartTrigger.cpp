// Fill out your copyright notice in the Description page of Project Settings.


#include "StartTrigger.h"
#include "ApproachingStorm.h"
#include "ThiefCharacter.h"
#include "ReboundComponent.h"

void AStartTrigger::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AStartTrigger::OnOverlapBegin);
}

void AStartTrigger::OnOverlapBegin(AActor* OverlappedActor, 
                      AActor* OtherActor)
{
    AThiefCharacter* OtherPawn = Cast<AThiefCharacter>(OtherActor);

    if (OtherPawn == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("NO PAWN"));
        return;
    }

    AApproachingStorm* approachingStorm = Cast<AApproachingStorm>(storm);

    if (approachingStorm == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("NO STORM"));
        return;
    }

    if (OtherPawn->GetReboundComp()->IsRebounding()) return;


        UE_LOG(LogTemp, Display, TEXT("RESETTING STORM"));
        approachingStorm->Reset(true);


}


