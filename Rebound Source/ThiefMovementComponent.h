// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/TimelineComponent.h"

#include "ThiefMovementComponent.generated.h"

/**
 * 
 */
UENUM()
enum EMovementState
{
  Walking     UMETA(DisplayName = "Walking"),
  Sliding      UMETA(DisplayName = "Sliding"),
  Dashing		UMETA(DisplayName = "Dashing"),
};

DECLARE_DELEGATE(FOnDashDelegate);

UCLASS()
class REBOUND_API UThiefMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	public:
	UThiefMovementComponent();
	void SlidePressed();
	void SlideReleased();

	void DashPressed();
	void DashReleased();

	void BurstPressed();
	void BurstReleased();

	virtual bool DoJump(bool bReplayingMoves) override;

	void Plunge();

	uint8 SlideLevel = 0;
	uint8 DashLevel = 0;
	uint8 WalljumpLevel = 0;

	UPROPERTY(EditAnywhere)
	float JumpBoost = 0.f;

	void Walljump(FVector Normal);
	bool bPlayMovementSounds = true;

UFUNCTION()
	void TimelineProgress(float Value);

	bool IsInBurstMode();

	FOnDashDelegate OnDash;

	protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void CheckNewFootstep();

	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;

	// Timeline for transitioning camera pos and height between walking and sliding positions
	FTimeline SlideTimeline;
	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* CurveFloat;

	float StepProgress = 0.0f;

	

	

	private:
	
	
	bool bCanWalljump = true;

	FTimerHandle TimerHandle_WalljumpCooldown;
	UFUNCTION()
	void WalljumpCooldown() {bCanWalljump = true;};


	void ResolveMovementState();
	void SetMovementState(TEnumAsByte<EMovementState> NewState);
	void OnMovementStateChanged();
	FVector CalculateFloorInfluence(FVector FloorNormal);
	bool CanSlide();
	bool CanStand();

	void BeginSlide();
	void EndSlide();

	

	void BeginDash();
	UFUNCTION()
	void EndDash();

	

	

	/* Slide Cooldown Handling */
	bool bSlideAvailable = true;
	FTimerHandle TimerHandle_SlideCooldown;
	UFUNCTION()
	void SlideCooldown();

	/* Dash Duration Handling*/
	FTimerHandle TimerHandle_DashEnd;
	/* Calls EndDash() */
	

	TEnumAsByte<EMovementState> MovementState;

	bool bBurstKeyDown = false;
	bool bDashAvailable = true;
	bool bSecondDashAvailable = false;


	bool bSlideKeyDown = false;
	bool bSlideSFXQueued = false;
	float StandingCapsuleHalfHeight;
	float StandingCameraZOffset;
	float CurrentHalfHeight;

	float StandingBrakingFrictionFactor;
	float CrouchingBrakingFrictionFactor;

	float StandingFriction;
	float SlidingFriction;

	float WalkingAcceleration;

	float TimeSinceStateChange = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Jump Sound")
	class USoundBase* JSound;
	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Slide Sound")
	class USoundBase* SlideSound;
	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Walljump Sound")
	class USoundBase* WJSound;
	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Airhike Sound")
	class USoundBase* AirHikeSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Default Footsteps")
	TArray<class USoundBase*> FootstepSFX;

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Water Footsteps")
	TArray<class USoundBase*> WaterFootstepSFX;

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Landed Sound")
	TArray<USoundBase*> LandedSFX;

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Water Landed Sound")
	TArray<USoundBase*> WaterLandedSFX;

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Dash Sound")
	USoundBase* DashSFX;
};
