// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "ApproachingStorm.generated.h"

UCLASS()
class REBOUND_API AApproachingStorm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AApproachingStorm();

	UFUNCTION()
	void TimelineProgress(float Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SkyShift(bool Mode);


	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* AudioSource;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* ParticleSystem;

	// Timeline for transitioning player pos and height between walking and sliding positions
	FTimeline SizeTimeline;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* CurveFloat;

	float BaseYScale;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Reset(bool go = false);

	// when this shit gets close
	// I AM THE STORM THAT IS APPROOOOOAAAACHING

	int Difficulty = 3;// ranges from 0-6

private:

	AActor* PlayerActor;
	UPROPERTY(EditAnywhere, Category = "SFX", DisplayName = "Storm That is Approaching")
	class USoundBase* BTLMusic;

	bool bStormApproached = false;

	float BaseVelocity = 0.f;
	float Acceleration = 0.f;

	float TotalDeltaTime = 0.f;
	float Counts = 0.f;

	UPROPERTY(EditAnywhere, Category = "SFX", DisplayName = "Music source in scene");
	class AAmbientSound* MusicSource;

	FVector StartLocation;

	

};
