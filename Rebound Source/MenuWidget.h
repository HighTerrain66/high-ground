// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class REBOUND_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	protected:
	virtual bool Initialize();
	
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	class IMenuInterface* MenuInterface;
public:
	virtual void Setup();
	void Teardown();
private:
};
