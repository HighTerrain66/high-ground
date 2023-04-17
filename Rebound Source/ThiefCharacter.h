// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "TickDamageFloor.h"

#include "ThiefCharacter.generated.h"

DECLARE_DELEGATE_TwoParams(FOnHintReceivedDelegate, const FString&, float);

UCLASS()
class REBOUND_API AThiefCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera component that will be the viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
public:
	// Sets default values for this character's properties
	AThiefCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleDefaultsOnly)
	class UThiefMovementComponent* MoveComp;

	UPROPERTY(VisibleDefaultsOnly)
	class UUpgradeHandleComponent* UpgradeComp;

	UPROPERTY(VisibleDefaultsOnly)
	class UReboundComponent* ReboundComp;

	UPROPERTY(EditAnywhere)
	float MantleSpeed = 250.f;

	float ReboundProgress = 0.f;
	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* GetCamera() {return Camera;};

	UFUNCTION(BlueprintCallable)
	UUpgradeHandleComponent* GetUpgradeComp() {return UpgradeComp;};

	UFUNCTION(BlueprintCallable)
	UReboundComponent* GetReboundComp() {return ReboundComp;};

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetMesh() {return BaseMesh;};

	void OnJumped_Implementation() override;

	/** Trigger jump if jump button has been pressed. */
	void CheckJumpInput(float DeltaTime) override;

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(Exec)
	void TraceLoc();
	bool endIndex = false;

	UPROPERTY(BlueprintReadWrite)
	int32 Orbs = 0;

	UPROPERTY(BlueprintReadWrite)
	uint8 mantleLevel = 1;
	UPROPERTY(BlueprintReadWrite)
	uint8 walljumpLevel = 1;
	UPROPERTY(BlueprintReadWrite)
	uint8 slideLevel = 1;
	UPROPERTY(BlueprintReadWrite)
	uint8 boundLevel = 1;

	UFUNCTION(Category="HUD")
	void OnReceiveHint(const FString &Hint, float Time = 0.f);

	FString CurrentHint;

	FOnHintReceivedDelegate OnHint;

	UFUNCTION(BlueprintCallable)
	float GetReboundProgress() {return ReboundProgress;}

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void Turn(float AxisValue);

	void JumpPressed();
	void JumpReleased();

	UPROPERTY(EditAnywhere)
	float RotationRate = 100.f;

	bool bMantling = false;
	
	bool bJumpBindDown = false;
	bool bForwardBindDown = false;
	bool bCanWalljump = true;

	
	

	

	//bool bCanDash = true;
	void SetMantling(bool Value);
	bool JumpRayTrace(FHitResult& Hit, FVector Addon = FVector::ZeroVector, bool Backwards = false, FVector InputVector = FVector::ForwardVector);
	void Walljump(FVector Normal);
	
	FTimerHandle TimerHandle_WalljumpCooldown;
	UFUNCTION()
	void WalljumpCooldown() {bCanWalljump = true;};
	bool AttemptMantle();

	FTimerHandle TimerHandle_MantleNoiseCooldown;
	bool bMakeMantleNoise = true;

	UFUNCTION()
	void MantleNoiseCooldown() {bMakeMantleNoise = true;}

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Walljump Sound")
	class USoundBase* WJSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Jump Sound")
	class USoundBase* JSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Mantle Sound")
	class USoundBase* MSound;

	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Mantle Complete Sound")
	class USoundBase* MDoneSound;

};
