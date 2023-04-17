// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerLocationIfSeen.generated.h"

/**
 * 
 */
UCLASS()
class ADENSTAL_API UBTService_PlayerLocationIfSeen : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_PlayerLocationIfSeen();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void AssignAIValues(class UShooterCharacterAIComponent* AIComponent);

	bool init = true;
	int32 WaypointIndex = 0;
	int32 HighestWaypointIndex = 0;
	float WaitTime = 5.f;
	float CurrentWaitTime;
};
