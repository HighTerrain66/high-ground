// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "ConsoleTrigger1.generated.h"

/**
 * 
 */
UCLASS()
class ADENSTAL_API AConsoleTrigger1 : public ATriggerVolume
{
	GENERATED_BODY()
	
	protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* AudioComponent = nullptr;

	bool Activated = false;

	UPROPERTY(EditInstanceOnly)
	AActor* ActorThatOpens;
	public:

	AConsoleTrigger1();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	private:

	

	
};
