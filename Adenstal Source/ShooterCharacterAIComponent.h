// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCharacterAIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADENSTAL_API UShooterCharacterAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShooterCharacterAIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	TArray<FVector> GetWaypoints() const {return Waypoints;};

	float GetWaitTime() const {return WaitTime;};

	UPROPERTY(VisibleAnywhere)
	FString AIState = "unset";

	UPROPERTY(EditDefaultsOnly)
	float CombatMoveSpeed = 550.f;
	UPROPERTY(EditDefaultsOnly)
	float PatrolMoveSpeed = 180.f;

	UPROPERTY(EditInstanceOnly)
	bool bIsStatic = false;
private:
	UPROPERTY(EditInstanceOnly, Meta = (MakeEditWidget = true))
	TArray<FVector> Waypoints;
	UPROPERTY(EditInstanceOnly)
	float WaitTime = 5.f;

	

};
