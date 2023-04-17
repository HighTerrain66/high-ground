// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class ADENSTAL_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger(float Spread = 0);
	void ReleaseTrigger();
	void PullTrigger2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	int32 Ammo = 30;
	UPROPERTY(EditDefaultsOnly)
	int32 Ammo2 = 0;

	UFUNCTION(BlueprintPure)
	int32 GetAmmo1() const;
	UFUNCTION(BlueprintPure)
	int32 GetAmmo2() const;

	void SetIsOnGround(bool Value);
	bool GetIsOnGround() {return IsOnGround;};

	uint32 GetGunID() {return GunID;}

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	class USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* PickupSound;

	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectile> ProjectileClass;

	


	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 10000.f;

	

	UPROPERTY(EditAnywhere)
	float MuzzleVelocity = 0.f;

	

	UPROPERTY(EditAnywhere)
	float BaseDamage = 33.f;

	UPROPERTY(EditAnywhere)
	float ShotSeparation = 0.05f;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash2;

	UPROPERTY(EditAnywhere)
	class USoundBase* MuzzleSound2;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AProjectile> ProjectileClass2;

	UPROPERTY(EditAnywhere)
	float MaxRange2 = 3000.f;

	UPROPERTY(EditAnywhere)
	float MuzzleVelocity2 = 0.f;

	UPROPERTY(EditAnywhere)
	float BaseDamage2 = 33.f;

	UPROPERTY(EditAnywhere)
	float ShotSeparation2 = 0.3f;

	UPROPERTY(EditAnywhere)
	bool AutoFire = false;

	UPROPERTY(EditAnywhere)
	float BaseRecoil1 = 0.1f;
	UPROPERTY(EditAnywhere)
	float BaseRecoil2 = 0.5f;

	UPROPERTY(EditAnywhere)
	float SubsequentShotRecoilModifier = 1.5f;

	float CurrentRecoil = 0.f;

	UPROPERTY(EditAnywhere)
	float BaseHorizontalRecoil = 0.05f;
	UPROPERTY(EditAnywhere)
	float MaxHorizontalRecoil = 1.f;

	float CurrentHorizontalRecoil = 0.f;


	void HitScan(float Damage = 33.f, float Range = 3000.f, class USoundBase* Sound = nullptr, UParticleSystem* Effect = nullptr, float Spread = 0);
	void Projectile(float Damage = 33.f, float Range = 3000.f, TSubclassOf<class AProjectile> Class = nullptr);

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection, float Spread = 0);
	void Recoil();

	AController* GetOwnerController() const;

	UFUNCTION()
	void OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	float Load = ShotSeparation;
	UPROPERTY(VisibleAnywhere)
	float Load2 = ShotSeparation2;
	UPROPERTY(VisibleAnywhere)
	bool SustainedFire = false;

	UPROPERTY(EditAnywhere)
	bool IsOnGround = false;

	UPROPERTY(EditDefaultsOnly)
	uint32 GunID = 0;

	UPROPERTY(EditAnywhere)
	uint32 PickupAmmo1 = 15;
	UPROPERTY(EditAnywhere)
	uint32 PickupAmmo2 = 0;

	UFUNCTION()
	void CreateNoise(FVector Location, float Radius, uint8 Priority, AController* SourceController);

	UPROPERTY(EditAnywhere)
	float FireAudibleRange = 3000.f;
	UPROPERTY(EditAnywhere)
	float ImpactAudibleRange = 1500.f;


};
