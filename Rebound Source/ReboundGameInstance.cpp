// Fill out your copyright notice in the Description page of Project Settings.


#include "ReboundGameInstance.h"

#include "Engine/Engine.h"
#include "IPAddress.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.h"
#include "InGameMenu.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UReboundGameInstance::UReboundGameInstance(const FObjectInitializer & ObjectInitializer)
{
    
}

void UReboundGameInstance::Host(FString Name)
{
    
}

void UReboundGameInstance::Join(uint32 Index)
{
    
}

void UReboundGameInstance::StartSession()
{
    
}

void UReboundGameInstance::LoadMenu()
{
    
}

void UReboundGameInstance::LoadInGameMenu()
{
    
}

void UReboundGameInstance::QuitToMenu()
{
    
}

void UReboundGameInstance::RequestRefresh()
{
    
}

void UReboundGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
    
}

void UReboundGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
    
}

void UReboundGameInstance::OnFindSessionComplete(bool Success)
{
    
}

void UReboundGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    
}

void UReboundGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
    
}

