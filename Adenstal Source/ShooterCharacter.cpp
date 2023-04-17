// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "GameFramework/WorldSettings.h"
#include "Components\CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AdenstalGameModeBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "ShooterAIController.h"
#include "Animation/AnimMontage.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "GenericTeamAgentInterface.h"
#include "SoundStimuli.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdenstalGameModeBase.h"
#include "AIManager.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->SetRelativeLocation(FVector(-17.251709f, 80.644775f, 81.469589f));

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));

	SlashHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SlashHitbox"));
	SlashHitBox->SetupAttachment(GetCapsuleComponent());
	SlashHitBox->SetRelativeLocation(FVector(123, 0, 16));
	SlashHitBox->SetRelativeScale3D(FVector(2.6f, 4.3f, 1.7f));
	SlashHitBox->SetGenerateOverlapEvents(true);
	SlashHitBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	//SlashHitBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_l"));
	//StimuliSource->bAutoRegisterAsSource = true;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Voice"));
	AudioComp->SetupAttachment(GetCapsuleComponent());


	
	bCanDeclareContact = true;
	//(X=-17.251709,Y=80.644775,Z=81.469589)
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	Gun = GetWorld()->SpawnActor<AGun>(StarterGunClass);

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);

	

	UGameplayStatics::GetAllActorsOfClass(this, APawn::StaticClass(), TargetPawns);
	TargetPawns.Remove(this);
	//Gun->SetActorOffset(FVector(0.070383f, -14.885326f, -1.164200f));
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// shift view up with recoil
	AddControllerPitchInput(-RecoilInputVertical);
	AddControllerYawInput(-RecoilInputHorizontal);
	RecoilInputVertical *= 0.8f;
	RecoilInputHorizontal *= 0.6f;
	if (RecoilInputVertical < 0.001) RecoilInputVertical = 0;
	if (RecoilInputHorizontal < 0.001) RecoilInputHorizontal = 0;



	// Spark caster targeting
	if (bIsTargeting)
	{
		// Find closest target to centre screen
		float HighestDot = 0.9995f;

		FVector ViewportPos;
		FRotator Path;
		if (GetController() == nullptr) return;
		GetController()->GetPlayerViewPoint(ViewportPos, Path);
		for (AActor* PotentialTarget : TargetPawns)
		{
			if (Cast<AShooterCharacter>(PotentialTarget))
			{
				if (Cast<AShooterCharacter>(PotentialTarget)->IsDead())
				{
					continue;
				}
			}
			if (GetController()->LineOfSightTo(PotentialTarget))
			{
				float Dot = FVector::DotProduct(Path.Vector(), (PotentialTarget->GetActorLocation() - ViewportPos).GetSafeNormal());
				
				if (Dot > HighestDot)
				{
					HighestDot = Dot;
					if (LockedTarget == nullptr) UGameplayStatics::SpawnSoundAttached(TargetingSound, GetMesh());
					
					LockedTarget = PotentialTarget;
					
				}
				else if (PotentialTarget == LockedTarget && Dot < 0.6f)
				{
					// unlock target if looking far enough away from locked target
					LockedTarget = nullptr;
				}
			}
			else if (PotentialTarget == LockedTarget)
			{
				LockedTarget = nullptr;
			}
		}
		
		if (LockedTarget)
		{
			//UE_LOG(LogTemp, Display, TEXT("LOCKED %s"), *LockedTarget->GetName());
			DrawDebugLine(GetWorld(), GetMesh()->GetSocketLocation(TEXT("canon_barrel_C")), LockedTarget->GetActorLocation(), FColor::Red, false);
		}
	}


	if (bIsInCombat && GetWorldTimerManager().GetTimerRemaining(TimerHandle_OutOfCombat) == -1.f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_OutOfCombat, this, &AShooterCharacter::ExitCombatAfterSprint, 9.f, false, 9.f);
		//UE_LOG(LogTemp, Display, TEXT("EXITING COMBAT, %f"), GetWorldTimerManager().GetTimerRemaining(TimerHandle_OutOfCombat));
		
	}

	//Healing
	if (HealthToRegen > 0)
	{
		Health += HealthToRegen * DeltaTime;
		if (Health > MaxHealth) Health = MaxHealth;
		HealthToRegen -= 5 * DeltaTime;
	}
	
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsInvincible) return 0.f;
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsDead())
	{
		return 0.f;
	}
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	
	//UE_LOG(LogTemp, Warning, TEXT("THAT'S A LOTTA DAMAGE\nHealth remaining: %f"), Health)

	if (IsDead())
	{
		//broadcast pawn death to the gamemode
		AAdenstalGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AAdenstalGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		CalculateDamageDirection(DamageCauser->GetActorLocation());
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetGun()->SetIsOnGround(true);
		GetWorldTimerManager().ClearAllTimersForObject(this);
		StopAnimMontage(GetCurrentMontage());
		if (DeathSound)
		{
			AudioComp->SetSound(DeathSound);
			AudioComp->Play();
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), 1.f);
		}
		
	}
	else if (!bIsTargeting && GetCurrentMontage() != SlashMontage)
	{
		PlayAnimMontage(HitMontages[CalculateDamageDirection(DamageCauser->GetActorLocation())]);
	}


	AAIController* COMController = Cast<AAIController>(GetController());
	if (HitCameraShakeClass)
	{
		if (GetController() == GetWorld()->GetFirstPlayerController())
		{
			Cast<APlayerController>(GetController())->ClientStartCameraShake(HitCameraShakeClass);
		}
	}
	if (COMController)
	{
		// detect player if an ai is shot at
		//UE_LOG(LogTemp, Display, TEXT("AI Hit %s"), *COMController->GetName());
		Cast<AShooterAIController>(COMController)->OnTakeDamage(DamageCauser);
	}

	return DamageToApply;
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::TriggerPull);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &AShooterCharacter::TriggerRelease);
	PlayerInputComponent->BindAction(TEXT("Fire2"), EInputEvent::IE_Pressed, this, &AShooterCharacter::TriggerPull2);
	PlayerInputComponent->BindAction(TEXT("Ability1"), EInputEvent::IE_Pressed, this, &AShooterCharacter::StartSlash);
	PlayerInputComponent->BindAction(TEXT("Ability2"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SparkCaster);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AShooterCharacter::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AShooterCharacter::EndSprint);
	PlayerInputComponent->BindAction(TEXT("Heal"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Heal);

}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
	if (AxisValue > 0.3f) bTurnRight = true;
	else bTurnRight = false;

	if (AxisValue < -0.3f) bTurnLeft = true;
	else bTurnLeft = false;
}

void AShooterCharacter::Heal() 
{
	if (Heals < 1) return;
	--Heals;
	//Health += 50;
	HealthToRegen += 20.f;
	if (HealthToRegen > 5.f) HealthToRegen -= 5.f;
	//if (Health > MaxHealth) Health = MaxHealth;
}


void AShooterCharacter::SparkCaster() 
{
	if (!bIsActionable) return;
	
	Gun->ReleaseTrigger();
	bIsTargeting = true;
	bIsInCombat = true;
	bIsActionable = false;
	if (SparkCasterFireMontage != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("RCOSS MOMENT"));
		PlayAnimMontage(SparkCasterFireMontage);
	}
	UE_LOG(LogTemp, Display, TEXT("R'COSS MOMENT"));
	GetWorldTimerManager().SetTimer(TimerHandle_FireSparkCaster, this, &AShooterCharacter::FireSparkCaster, 1.70f, false, 1.70f);
	GetWorldTimerManager().SetTimer(TimerHandle_BecomeActionable, this, &AShooterCharacter::BecomeActionable, 1.8f, false, 2.f);
	
}

void AShooterCharacter::FireSparkCaster() 
{
	bIsTargeting = false;
	if (!SparkCasterBolt) return;
	FRotator Trajectory;
	if (LockedTarget)
	{
		Trajectory = (LockedTarget->GetActorLocation() - GetActorLocation()).Rotation();
	}
	else
	{
		FVector NotNeeded;
		FRotator Path;
		GetController()->GetPlayerViewPoint(NotNeeded, Path);
		Path += FRotator(0.f, 3.f, 0.f);

		Trajectory = Path;
	}
	LockedTarget = nullptr;

	

	FTransform CannonTransform = GetMesh()->GetSocketTransform(TEXT("canon_barrel_C"));

	if (SparkCasterMuzzleFlash && SparkCasterFireSound)
	{
		UGameplayStatics::SpawnEmitterAttached(SparkCasterMuzzleFlash, GetMesh(), TEXT("canon_barrel_C"), FVector::ZeroVector, Trajectory, FVector(1, 1, 1), EAttachLocation::KeepRelativeOffset);
		UGameplayStatics::SpawnSoundAttached(SparkCasterFireSound, GetMesh());
	}
	

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(SparkCasterBolt, CannonTransform.GetLocation() + GetActorForwardVector() * 75, Trajectory);
	Projectile->SetOwner(this);
	Projectile->SetActorScale3D(Projectile->GetActorScale3D() * 0.5f);
	//MoveIgnoreActorAdd(Projectile);

	
}

void AShooterCharacter::StartSprint() 
{
	if (bExitCombatAfterSprint) bIsInCombat = false;
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->MaxWalkSpeed = SprintSpeed;
}

void AShooterCharacter::EndSprint() 
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->MaxWalkSpeed = JogSpeed;
}

void AShooterCharacter::StartSlash() 
{
	if (!bIsActionable) return;
	
	Gun->ReleaseTrigger();
	bIsTargeting = true;
	bIsInCombat = true;
	bIsActionable = false;
	if (SlashMontage != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("RCOSS MOMENT"));
		PlayAnimMontage(SlashMontage);
	}
	UE_LOG(LogTemp, Display, TEXT("R'COSS MOMENT"));
	GetWorldTimerManager().SetTimer(TimerHandle_Slash, this, &AShooterCharacter::MidSlash, 1.0f, false, 1.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_BecomeActionable, this, &AShooterCharacter::BecomeActionable, 2.3f, false, 2.3f);
}

void AShooterCharacter::MidSlash() 
{
	if (SlashEffect && TPSound)
	{
		UGameplayStatics::SpawnEmitterAttached(SlashEffect, GetMesh(), TEXT("hand_l"), FVector(170.f, 0.f, 0.f), FRotator::ZeroRotator, FVector(1, 1, 1), EAttachLocation::KeepRelativeOffset);
		UGameplayStatics::SpawnSoundAttached(TPSound, GetMesh());
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_Slash, this, &AShooterCharacter::EndSlash, 0.7f, false, 0.7f);
	if (LockedTarget != nullptr && Cast<APlayerController>(GetController())->GetInputKeyTimeDown(EKeys::E) > 0.f)
	{
		FVector TargetLocation = LockedTarget->GetActorLocation() + (GetActorLocation() - LockedTarget->GetActorLocation()).GetSafeNormal() * 200.f;
		FHitResult* HitResult = nullptr;
		SetActorLocation(TargetLocation, false, HitResult, ETeleportType::ResetPhysics);
		AddActorLocalRotation(FRotator(0, 180.f, 0.f));
		Cast<UCharacterMovementComponent>(GetMovementComponent())->Velocity = FVector::ZeroVector;
	}

	
	bIsTargeting = false;
	LockedTarget = nullptr;
}

void AShooterCharacter::EndSlash() 
{
	UE_LOG(LogTemp, Display, TEXT("SLASH"));
	
	TArray<AActor*> Targets;
	if (SlashSound) UGameplayStatics::SpawnSoundAttached(SlashSound, GetMesh());

	if (SlashHitBox == nullptr) return;
	SlashHitBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	
	SlashHitBox->GetOverlappingActors(Targets, AShooterCharacter::StaticClass());// none?
	if (!Targets.IsValidIndex(0)) return;
	for (AActor* HitActor : Targets)
	{
		if (HitActor == this) continue;
		UE_LOG(LogTemp, Display, TEXT("HIT"));
		UGameplayStatics::ApplyDamage(HitActor, 100.f, GetController(), this, UDamageType::StaticClass());
		if (SlashImpact)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SlashImpact, HitActor->GetTransform());
		}
		
	}

	SlashHitBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

// 0 front 1 back 2 right 3 left
int AShooterCharacter::CalculateDamageDirection(FVector SourceLocation) 
{
	FVector RelativeLocation = GetTransform().InverseTransformPosition(SourceLocation);
	if (RelativeLocation.X > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("DEAD FROM FRONT"));
		bDeadFromFront = true;
		if (RelativeLocation.Y > RelativeLocation.X)
		{
			return 2;
		}
		else if (RelativeLocation.Y < -RelativeLocation.X)
		{
			return 3;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("DEAD FROM BACK"));
		bDeadFromFront = false;
		if (RelativeLocation.Y < RelativeLocation.X)
		{
			return 3;
		}
		else if (RelativeLocation.Y > -RelativeLocation.X)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}
}

void AShooterCharacter::ReportNoise(USoundBase* SoundToPlay, float Volume) 
{
	//If we have a valid sound to play, play the sound and
	//report it to our game
	if (SoundToPlay)
	{
		//Play the actual sound
		AudioComp->SetSound(SoundToPlay);
		AudioComp->Play();
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation(), Volume);
 
		//Report that we've played a sound with a certain volume in a specific location
		MakeNoise(Volume, this, GetActorLocation());
		USoundStimuli::CreateNoise(GetActorLocation(), 3000.f, 2, this);


	}
	
}

void AShooterCharacter::whosthere(bool Value) 
{
	AAdenstalGameModeBase* GameMode = Cast<AAdenstalGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->GetAIManager()->whosthere(Value);
	}
}



void AShooterCharacter::BecomeActionable() 
{
	bIsActionable = true;
	//UE_LOG(LogTemp, Display, TEXT("RECOVERED"));
}

void AShooterCharacter::EnterCombat() 
{
	bIsInCombat = true;
	bExitCombatAfterSprint = false;
}

void AShooterCharacter::ExitCombatAfterSprint() 
{
	bExitCombatAfterSprint = true;
	if (GetCharacterMovement()->MaxWalkSpeed == SprintSpeed) 
	{ 
		bIsInCombat = false;
	}
}

void AShooterCharacter::TriggerPull()
{
	if (bIsInCombat && bIsActionable)
	{
		Gun->PullTrigger(GetFiringSpread());
	}
	
}

void AShooterCharacter::TriggerRelease()
{
	EnterCombat();
	//UE_LOG(LogTemp, Display, TEXT("TRIGGER RELEASED"));
	Gun->ReleaseTrigger();
}

void AShooterCharacter::TriggerPull2()
{
	if (bIsInCombat && bIsActionable)
	{
		Gun->PullTrigger2();
	}
	EnterCombat();
}

bool AShooterCharacter::IsDead() const
{
	if (Health > 0)
	{
		return false;
	}
	return true;
}

bool AShooterCharacter::IsInCombat() const
{
	return bIsInCombat;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health/MaxHealth;
}

AGun* AShooterCharacter::GetGun() const
{
	//UE_LOG(LogTemp, Display, TEXT("GUN"));
	return Gun;
}

void AShooterCharacter::DeclareContact()
{
	UE_LOG(LogTemp, Warning, TEXT("CONTACT!"));

	if(GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("CONTACT"));
	}
	
	if (bCanDeclareContact)
    {
        // Set bool to false so we don't hit this logic multiple times per tick
        bCanDeclareContact = false;
		EnterCombat();

        if (SpottedSound)
        {
            ReportNoise(SpottedSound, 1.f);

        }
 
        // Set timer for 2 seconds before we are able to repeat declare contact
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_DeclareContact, this, &AShooterCharacter::PlayDeclareContact, 0.f, false, 2.f);
    }
}

void AShooterCharacter::DetectNoise() 
{

	if (bCanReportNoise && !IsDead())
    {
		if(GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("NOISE"));
		}
        // Set bool to false so we don't hit this logic multiple times per tick
        bCanReportNoise = false;
		EnterCombat();

        if (NoiseDetectedSound)
		{
			AudioComp->SetSound(NoiseDetectedSound);
			AudioComp->Play();
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoiseDetectedSound, GetActorLocation(), 1.f);
			
            //ReportNoise(SpottedSound, 1.f);

        }
 
        // Set timer for 2 seconds before we are able to repeat detect noise
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReportNoise, this, &AShooterCharacter::PlayReportNoise, 0.f, false, 2.f);
    }
}

/* #include "GameFramework/WorldSettings.h"
void ADIO::StopTime()
{
	UWorld* ZaWarudo = GetWorld();
	ZaWarudo->GetWorldSettings()->SetTimeDilation(0.f);
} */


