// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleTriggerReac1.h"
#include "Math/Vector2D.h"

void AConsoleTriggerReac1::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) 
{
    Super::OnOverlapBegin(OverlappedActor, OtherActor);

    if (!Activated && OtherActor == ActorThatOpens)
    {
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, TEXT("!REACTOR SAFETY MEASURES OVERRIDDEN!"), true, FVector2D(2, 2));
        }

        Activated = true;
    }

}
