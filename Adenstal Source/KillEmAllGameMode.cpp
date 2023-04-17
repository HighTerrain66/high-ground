// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "ShooterCharacter.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
    Super::PawnKilled(PawnKilled);
    UE_LOG(LogTemp, Error, TEXT("PAWN KILLED!"));

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("TRYING TO END"));
        Cast<AShooterCharacter>(PawnKilled)->TriggerRelease();
        EndGame(false);
        //PlayerController->GameHasEnded(nullptr, false);
    }

    //for loop over shooter ai in world, if none are dead, win the game
    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!Controller->IsDead()) {return;}
    }
    EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        Controller->GameHasEnded(Controller->GetPawn(), Controller->IsPlayerController() == bIsPlayerWinner);
    }
}
