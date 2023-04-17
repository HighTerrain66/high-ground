// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIManager.generated.h"



UCLASS()
class ADENSTAL_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(exec)
	void whosthere(bool Value = true);

private:
	TArray<class AActor*> Subordinates;

	UFUNCTION()
	void ReceiveFirstAlert();

	int32 GruntsAlerted = 0;

	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere)
	class USoundBase* AmbientSound;
	UPROPERTY(EditAnywhere)
	class USoundBase* AlertMusic;
};
