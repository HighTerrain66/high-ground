// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class REBOUND_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//targetlocation is visible as a widget
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 125;

	void AddActiveTrigger();
	void RemoveActiveTrigger();
	
protected:
	UPROPERTY(EditAnywhere)
	int ActiveTriggers = 1;

private:
	
	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;
	
	FVector MovementVector;

	
	
};
