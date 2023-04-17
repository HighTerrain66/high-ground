// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Components/TimelineComponent.h"

#include "ReboundComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBOUND_API UReboundComponent : public UActorComponent
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this component's properties
	UReboundComponent();

	void StartRebound();

	UFUNCTION()
	void TimelineProgress(float Value);

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	class AThiefCharacter* PlayerCharacter;
	UPROPERTY()
	class UThiefMovementComponent* PlayerMovement;

	// Timeline for transitioning player pos and height between walking and sliding positions
	FTimeline ReboundTimeline;

	// Curve float for rebound speed depending on distance from target when starting rebound
	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* CurveFloat;

	// Where the current rebound started from
	FVector ReboundStartLocation;
	// Where to rebound back to
	FVector ReboundTarget;

	// True when moving back with a rebound- orb damage hitboxes don't take effect
	bool bIsRebounding = false;

	// Storm actor
	UPROPERTY()
	class AApproachingStorm* HazardField;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* ReboundSFX;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsRebounding() {return bIsRebounding;};
};
