// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsoleTrigger1.h"
#include "ConsoleTriggerFirst.generated.h"

/**
 * 
 */
UCLASS()
class ADENSTAL_API AConsoleTriggerFirst : public AConsoleTrigger1
{
	GENERATED_BODY()
	
	public:

	AConsoleTriggerFirst();
	
	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ClassToSpawn;
};
