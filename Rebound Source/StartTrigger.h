// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "StartTrigger.generated.h"

/**
 * 
 */
UCLASS()
class REBOUND_API AStartTrigger : public ATriggerBox
{
	GENERATED_BODY()
public:
UPROPERTY(EditAnywhere)
AActor* storm;

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	
};
