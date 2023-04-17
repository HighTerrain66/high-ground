// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

bool UMenuWidget::Initialize() 
{
    bool Success = Super::Initialize();
    
    if (!Success) {return false;}

    /* MenuInterface = Cast<IMenuInterface>(GetWorld()->GetGameInstance());
    if (MenuInterface == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("OH DEWSS IS BAD"));
        return false;
    } */

    return true;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) 
{
    Super::OnLevelRemovedFromWorld(InLevel, InWorld);

    Teardown();
}

void UMenuWidget::Setup() 
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController == nullptr) {return;}

    FInputModeUIOnly InputMode = FInputModeUIOnly();
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetWidgetToFocus(TakeWidget());


    PlayerController->SetInputMode(InputMode);
    PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Teardown() 
{
    this->RemoveFromViewport();

    UWorld* World = GetWorld();
    if (World == nullptr) {return;}

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (PlayerController == nullptr) {return;}

    FInputModeGameOnly InputModeData;
    PlayerController->SetInputMode(InputModeData);

    PlayerController->bShowMouseCursor = false;
}