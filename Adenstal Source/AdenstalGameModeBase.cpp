// Copyright Epic Games, Inc. All Rights Reserved.


#include "AdenstalGameModeBase.h"
#include "AIManager.h"

void AAdenstalGameModeBase::PawnKilled(APawn* PawnKilled)
{
    
}
void AAdenstalGameModeBase::StartPlay() 
{
    Super::StartPlay();
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Name = TEXT("AI Manager");

    UClass* Class;
    UE_LOG(LogTemp, Error, TEXT("Oh"));
    if (ManagerToSpawn)
    {
        UE_LOG(LogTemp, Error, TEXT("Yes"));
        Class = ManagerToSpawn.Get();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No AI manager template found!"));
        Class = AAIManager::StaticClass();
    }
    
    AIManager = GetWorld()->SpawnActor<AAIManager>(Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
}
