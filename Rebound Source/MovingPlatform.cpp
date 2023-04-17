// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform() 
{
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay() 
{
    Super::BeginPlay();
    
    // replicate this actor from server to clients
    if (HasAuthority())
    {
        SetReplicates(true);
        SetReplicateMovement(true);
    }
    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
    MovementVector = (TargetLocation).GetSafeNormal();
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (HasAuthority())
    {
        if (ActiveTriggers > 0)
        {
            FVector Location = GetActorLocation();
            FVector Displacement = GlobalTargetLocation - GlobalStartLocation;
            FVector Direction = Displacement.GetSafeNormal();
            Location += MoveSpeed * DeltaTime * Direction;

            SetActorLocation(Location);

            // if the platform has travelled beyond the targetlocation
            if ((Location - GlobalStartLocation).SizeSquared() > (GlobalTargetLocation - GlobalStartLocation).SizeSquared())
            {
                GlobalTargetLocation = GlobalStartLocation;
                GlobalStartLocation = Location;
            }
        }
        
    }
    

    
}

void AMovingPlatform::AddActiveTrigger() 
{
    ActiveTriggers ++;
}

void AMovingPlatform::RemoveActiveTrigger() 
{
    if (ActiveTriggers > 0) {ActiveTriggers --;}
    else {UE_LOG(LogTemp, Error, TEXT("Platform %s has negative active triggers!"), *GetName())}
}

