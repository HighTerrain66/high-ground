// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsoleTrigger1.h"
#include "ConsoleTriggerReac1.generated.h"

/**
 * 
 */
UCLASS()
class ADENSTAL_API AConsoleTriggerReac1 : public AConsoleTrigger1
{
	GENERATED_BODY()
	
	public:
	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) override;
};
