// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AdenstalGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ADENSTAL_API AAdenstalGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void StartPlay() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AAIManager> ManagerToSpawn;
public:
	virtual void PawnKilled(APawn* PawnKilled);
	AAIManager* GetAIManager() {return AIManager;};
private:
	AAIManager* AIManager;

	
};
