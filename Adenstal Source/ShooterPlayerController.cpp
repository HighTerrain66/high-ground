// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, false);
    UE_LOG(LogTemp, Error, TEXT("END OF ZA WARUDO"));


    if (HUD != nullptr)
    {
        HUD->RemoveFromViewport();
    }

    if (bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass, TEXT("WinScreen"));
        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass, TEXT("LoseScreen"));
        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
    
}



void AShooterPlayerController::BeginPlay() 
{
    HUD = CreateWidget(this, HUDClass, TEXT("HUD"));
    if (HUD != nullptr)
    {
        HUD->AddToViewport();
    }

    //SetAudioListenerOverride(Cast<USceneComponent>(GetPawn()->FindComponentByClass(USkeletalMeshComponent::StaticClass())), FVector(0, 0, 0), FRotator(0, 0, 0));
}
