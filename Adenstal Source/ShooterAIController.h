// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UENUM()
enum AIState
{
	LOITER,
	PATROL,
	CHASE,// used to get within effective range of enemy
	ATTACK,// used within effective range of enemy
	INVESTIGATE,// ised to check out suspected enemy location
};

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFirstAlert);

UCLASS()
class ADENSTAL_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
public:
	// Called every frame
	AShooterAIController();
	virtual void Tick(float DeltaTime) override;
	bool IsDead() const;

	void OnTakeDamage(AActor* Cause);

	void BeginInvestigation(FVector Position);
	
	/* void PerceptionUpdate(const TArray<AActor*>& UpdatedActors); */

	void RegisterNoise(FVector Location, uint8 Priority = 0, AActor* SourceActor = nullptr, FString Reason = "Default");

	bool bIsBlind = false;

	FOnFirstAlert FirstAlert;


protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	//FGenericTeamId TeamId;
	//virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
	UFUNCTION()
	void AITick();

	bool IsTargetStillInSight();
	// looks for threats from a noncombat state
	void Perception();

	bool CanSeeActor_NonCombat(AActor* Actor) const;

	class AActor* Target;
	FVector StartLocation;
	FVector PatrolLocation;
	FVector InvestigationLocation;

	uint8 CurrentInvestigationPriority = 0;
	

	float CombatMoveSpeed = 500;
	float PatrolMoveSpeed = 150;

	AIState CurrentState;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* AIBehavior;

	FTimerHandle TimerHandle_AITick;

	void Loiter();
	void Patrol();
	void Chase();
	void Attack();
	void Investigate();

	// reverts to default behaviour
	void BreakOff();
	// goes into combat behaviour
	void BeginEngagement();

	UFUNCTION()
	void AIAttack();

	// Sets the next patrol location
	UFUNCTION()
	void NextPatrolPoint();

	FTimerHandle TimerHandle_AIAttack;
	FTimerHandle TimerHandle_BreakOff;
	FTimerHandle TimerHandle_NextPatrolPoint;

	

	// AI Parameters from AIComponent attached to character
	void AssignAIValues(class UShooterCharacterAIComponent* AIComponent);
	int32 WaypointIndex = 0;
	int32 HighestWaypointIndex = 0;
	float WaitTime = 5.f;
	float CurrentWaitTime;

	AIState OldState;
	bool bIsStatic = false;
	bool bHasEngagedYet = false;

	class UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* FireMontage;

};
