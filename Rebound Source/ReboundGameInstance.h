// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ReboundGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REBOUND_API UReboundGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	public:
	UReboundGameInstance(const FObjectInitializer & ObjectInitializer);

	UFUNCTION(Exec)
	virtual void Host(FString Name = TEXT("New Server"));

	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;

	void StartSession();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION()
	virtual void QuitToMenu() override;

	virtual void RequestRefresh() override;
	private:
	TSubclassOf<class UUserWidget> MenuClass;

	TSubclassOf<UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool Success);

	UFUNCTION()
	void OnDestroySessionComplete(FName SessionName, bool Success);

	UFUNCTION()
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
};
