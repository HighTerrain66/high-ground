// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThiefCharacter.h"
#include "UpgradeHandleComponent.h"
#include "Components/TimelineComponent.h"

#include "TickDamageFloor.generated.h"

UCLASS()
class REBOUND_API ATickDamageFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATickDamageFloor();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BaseMesh;

	// Hitbox for orb damage effects
	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageZone;

	class AThiefCharacter* ContactedCharacter;

	// Builds as player stays in contact with DamageZone
	float DamageBuildup = 0.f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
