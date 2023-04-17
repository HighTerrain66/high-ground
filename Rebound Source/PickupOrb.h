// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupOrb.generated.h"

UCLASS()
class REBOUND_API APickupOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Collider;

	UPROPERTY(EditAnywhere)
	int32 Value = 20;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	USoundBase* PickupSFX;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
