// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ReboundHUD.generated.h"

/**
 * 
 */
UCLASS()
class REBOUND_API AReboundHUD : public AHUD
{
	GENERATED_BODY()
	
	public:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  	TSubclassOf<class UReboundHUDWidget> HUDWidgetClass;


	private:

	UReboundHUDWidget* HUDWidget;
};
