// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundStimuli.generated.h"

/**
 * 
 */
UCLASS()
class ADENSTAL_API USoundStimuli : public UObject
{
	GENERATED_BODY()

public:
	USoundStimuli();
	~USoundStimuli();

	UFUNCTION(BlueprintCallable)
	static void CreateNoise(FVector Location, float Radius, uint8 Priority, class AActor* SourceActor, FString Reason = "Default");
};
