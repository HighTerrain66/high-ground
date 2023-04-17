// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReboundHUDWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class REBOUND_API UReboundHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnHintReceived(const FString& Hint, float Time);

	virtual bool Initialize() override;
};
