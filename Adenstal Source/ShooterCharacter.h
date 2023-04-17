// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gun.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GenericTeamAgentInterface.h"
#include "ShooterCharacter.generated.h"

class AGun;
class USoundBase;

UENUM()
enum Side
{
	REDFORS,
	WHITEFORS,
	INDEPENDENT,
};

UCLASS()
class ADENSTAL_API AShooterCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComponent;

	

	

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	void TriggerPull();
	void TriggerPull2();
	void TriggerRelease();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsInCombat() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	UFUNCTION(BlueprintPure)
	AGun* GetGun() const;
	UFUNCTION(BlueprintPure)
	int32 GetHeals() const { return Heals;};

	void DeclareContact();
	void DetectNoise();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	int CalculateDamageDirection(FVector SourceLocation);

	void AddAmmo(uint32 Amount, uint32 GunID);

	void SetContactFlag(bool Value) {bContactFlag = Value;};

	float RecoilInputVertical = 0.f;
	float RecoilInputHorizontal = 0.f;
	//class UShooterCharacterAIComponent* GetAIComponent() {return }

	float GetFiringSpread() const {return FiringSpread;};
	void SetFiringSpread(float Value) {FiringSpread = Value;};

	UPROPERTY(BlueprintReadOnly)
	bool bTurnRight;
	UPROPERTY(BlueprintReadOnly)
	bool bTurnLeft;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTargeting = false;
	UPROPERTY(BlueprintReadOnly)
	AActor* LockedTarget;

	UFUNCTION(BlueprintPure)
	bool DeadFromFront() {return bDeadFromFront;};

	UFUNCTION(BlueprintCallable, Category = AI)
	void ReportNoise(USoundBase* SoundToPlay, float Volume);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 ID = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* SlashHitBox;
protected:
	FGenericTeamId TeamId;

	bool bIsInvincible = false;
public:
	FGenericTeamId GetGenericTeamID() const { return TeamId; }
	//Side GetSide() const { return Team; }
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<Side> Team = Side::INDEPENDENT;
	UFUNCTION(Exec)
	void Sundowner() {bIsInvincible = !bIsInvincible;};

	UFUNCTION(Exec)
	void whosthere(bool Value = true);

	UFUNCTION()
	void EnterCombat();

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	UFUNCTION()
	void Heal();
	UFUNCTION()
	void SparkCaster();
	UFUNCTION()
	void FireSparkCaster();
	UFUNCTION()
	void StartSprint();
	UFUNCTION()
	void EndSprint();
	UFUNCTION()
	void StartSlash();
	UFUNCTION()
	void MidSlash();
	UFUNCTION()
	void EndSlash();

	

	UPROPERTY(EditDefaultsOnly)
	float JogSpeed = 910.0;
	UPROPERTY(EditDefaultsOnly)
	float SprintSpeed = 1300.f;

	UPROPERTY(EditAnywhere)
	float RotationRate = 8.f;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere)
	float Health = 1;
	float HealthToRegen = 0.f;

	UPROPERTY(VisibleAnywhere)
	int32 Heals = 3;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> GunArray[3];

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> StarterGunClass;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> SparkCasterBolt;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* SparkCasterMuzzleFlash;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* SlashEffect;

	//SFX --------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	class USoundBase* SparkCasterFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	class USoundBase* TargetingSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	class USoundBase* SlashSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	class USoundBase* TPSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMatineeCameraShake> HitCameraShakeClass;

	UPROPERTY(EditAnywhere, Category = "AI Voice Lines")
	class USoundBase* SpottedSound;

	UPROPERTY(EditAnywhere, Category = "AI Voice Lines")
	class USoundBase* NoiseDetectedSound;

	UPROPERTY(EditAnywhere, Category = "AI Voice Lines")
	class USoundBase* DeathSound;
	//END SFX -----------------------------------------------------

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TArray<class UAnimMontage*> HitMontages;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* HitFrontMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* SparkCasterFireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimMontage* SlashMontage;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* SlashImpact;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSource;

	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* AudioComp;

	bool bCanDeclareContact;
	bool bContactFlag;
	bool bCanReportNoise = true;
	// Called 10 seconds after contact has been declared
	UFUNCTION()
	void PlayDeclareContact() {bCanDeclareContact = true;};
	UFUNCTION()
	void PlayReportNoise() {bCanReportNoise = true;};
	FTimerHandle TimerHandle_DeclareContact;
	FTimerHandle TimerHandle_ReportNoise;
	FTimerHandle TimerHandle_BecomeActionable;
	void BecomeActionable();

	FTimerHandle TimerHandle_FireSparkCaster;
	FTimerHandle TimerHandle_OutOfCombat;
	FTimerHandle TimerHandle_Slash;


	float FiringSpread = 0;

	UPROPERTY(VisibleInstanceOnly)
	bool bIsInCombat = false;
	
	bool bIsActionable = true;
	bool bExitCombatAfterSprint = true;
	UFUNCTION()
	void ExitCombatAfterSprint();

	TArray<AActor*> TargetPawns;

	// whether character was killed from front or behind
	bool bDeadFromFront = true;
	
};
