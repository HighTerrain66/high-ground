// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "ThiefCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class REBOUND_API UThiefCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
	public:
	UThiefCameraComponent();

	protected:

	UPROPERTY()
	float BaseFOV = 102.571426f;

	UPROPERTY()
	float MaxFOV = 134.f;

	class UThiefMovementComponent* Movement;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void DashEffect();

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* ParticleSystem;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* SystemEffect;

	

	FTimerHandle TimerHandle_DashEffectEnd;
	UFUNCTION()
	void DashEffectEnd();
};
