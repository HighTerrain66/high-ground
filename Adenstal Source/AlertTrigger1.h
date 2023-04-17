// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConsoleTrigger1.h"
#include "AlertTrigger1.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ADENSTAL_API AAlertTrigger1 : public AConsoleTrigger1
{
	GENERATED_BODY()
	protected:
	virtual void BeginPlay() override;
	public:

	AAlertTrigger1();
	
	UFUNCTION()
	void OverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	private:

	UPROPERTY(EditAnywhere)
	class USoundBase* AlertMusic;
};
