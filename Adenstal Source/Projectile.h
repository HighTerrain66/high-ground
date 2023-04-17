// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ADENSTAL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	UPROPERTY(EditDefaultsOnly)
	float InitVelocity = 5000.f;

	

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleTrail;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float ExplosiveRadius = 0.f;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere)
	class USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	float AudibleRadius = 2500.f;

	UFUNCTION()
	void BecomeCollidable();
	FTimerHandle TimerHandle_Collidable;
};
