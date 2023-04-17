// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefCharacter.h"
#include "UObject/UObjectGlobals.h"
#include "Components/MeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "ThiefMovementComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ThiefCameraComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "UpgradeHandleComponent.h"
#include "ReboundComponent.h"
#include "ReboundHUDWidget.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

//plan for mantling is to use two raytraces

// Sets default values
AThiefCharacter::AThiefCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName).SetDefaultSubobjectClass<UThiefMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ACharacter::MeshComponentName);
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	// Create camera component 
	Camera = CreateDefaultSubobject<UThiefCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	UpgradeComp = CreateDefaultSubobject<UUpgradeHandleComponent>(TEXT("Upgrade Handle"));

	ReboundComp = CreateDefaultSubobject<UReboundComponent>(TEXT("Rebound Component"));
	


}

// Called when the game starts or when spawned
void AThiefCharacter::BeginPlay()
{
	Super::BeginPlay();
	

	//UReboundHUDWidget* HUD = Cast<UReboundHUDWidget>(Cast<APlayerController>(GetController())->GetHUD());
	//OnHint.BindUObject(HUD, &UReboundHUDWidget::OnHintReceived);
	//OnReceiveHint("YES");
	
}



// Called every frame
void AThiefCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMantling)
	{
		// lazy code for making mantle upgrade have you climb faster
		if (GetUpgradeComp()->mantleLevel == 2)
		{
			MantleSpeed = 450.f;
		}
		

		AddActorLocalOffset(FVector::UpVector * MantleSpeed * DeltaTime, true);
		FHitResult Hit;
		if (!JumpRayTrace(Hit, GetActorUpVector() * -100, false, GetActorForwardVector()))
		{
			SetMantling(false);
			GetCharacterMovement()->AddImpulse(GetActorForwardVector() * MantleSpeed / 5, true);

			if (GetUpgradeComp()->mantleLevel == 2)
			{
				GetCharacterMovement()->AddImpulse(GetActorForwardVector() * MantleSpeed - GetActorUpVector() * GetCharacterMovement()->GetLastUpdateVelocity().Z, true);
				
			}

			if (MDoneSound && bMakeMantleNoise)
			{
				UGameplayStatics::SpawnSoundAttached(MDoneSound, GetCamera());
				GetWorldTimerManager().SetTimer(TimerHandle_MantleNoiseCooldown, this, &AThiefCharacter::MantleNoiseCooldown, 0.2f, false, 0.2f);

				bMakeMantleNoise = false;
			}
			
		}

	}
	else if (bJumpBindDown)
	{
		AttemptMantle();
	}

	float tabHeldTime = Cast<APlayerController>(GetController())->GetInputKeyTimeDown(FKey("Tab"));

	if (tabHeldTime >= 2.0f)
	{
		ReboundComp->StartRebound();
		ReboundProgress = 1.0f;
	}
	else if (tabHeldTime >= 0.0f)
	{
		ReboundProgress = tabHeldTime / 2.0f;
	}
	else
	{
		ReboundProgress = 0.0f;
	}
}

// Called to bind functionality to input
void AThiefCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AThiefCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AThiefCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AThiefCharacter::Turn);

	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AThiefCharacter::LookUpRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AThiefCharacter::JumpPressed);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AThiefCharacter::JumpReleased);

	PlayerInputComponent->BindAction(TEXT("Slide"), EInputEvent::IE_Pressed, Cast<UThiefMovementComponent>(GetCharacterMovement()), &UThiefMovementComponent::SlidePressed);
	PlayerInputComponent->BindAction(TEXT("Slide"), EInputEvent::IE_Released, Cast<UThiefMovementComponent>(GetCharacterMovement()), &UThiefMovementComponent::SlideReleased);

	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, Cast<UThiefMovementComponent>(GetCharacterMovement()), &UThiefMovementComponent::DashPressed);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Released, Cast<UThiefMovementComponent>(GetCharacterMovement()), &UThiefMovementComponent::DashReleased);
	

	PlayerInputComponent->BindAction(TEXT("BurstMode"), EInputEvent::IE_Pressed, Cast<UThiefMovementComponent>(GetCharacterMovement()), &UThiefMovementComponent::BurstPressed);
	PlayerInputComponent->BindAction(TEXT("BurstMode"), EInputEvent::IE_Released, Cast<UThiefMovementComponent>(GetCharacterMovement()), &UThiefMovementComponent::BurstReleased);

	PlayerInputComponent->BindAction(TEXT("MenuRight"), EInputEvent::IE_Pressed, UpgradeComp, &UUpgradeHandleComponent::ShopRight);
	PlayerInputComponent->BindAction(TEXT("MenuLeft"), EInputEvent::IE_Pressed, UpgradeComp, &UUpgradeHandleComponent::ShopLeft);
	PlayerInputComponent->BindAction(TEXT("MenuAccept"), EInputEvent::IE_Pressed, UpgradeComp, &UUpgradeHandleComponent::ShopAccept);
	PlayerInputComponent->BindAction(TEXT("CheatBuyAll"), EInputEvent::IE_Pressed, UpgradeComp, &UUpgradeHandleComponent::CheatBuyAll);
}

void AThiefCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	//GetCharacterMovement()->AddImpulse(GetCharacterMovement()->GetLastInputVector() * 250, true);
}

// Full override of ACharacter function
void AThiefCharacter::CheckJumpInput(float DeltaTime)
{
	JumpCurrentCountPreJump = JumpCurrentCount;

	if (GetCharacterMovement())
	{
		if (bPressedJump)
		{
			// If this is the first jump and we're already falling,
			// then increment the JumpCount to compensate.
			const bool bFirstJump = JumpCurrentCount == 0;
			if (bFirstJump && GetCharacterMovement()->IsFalling())
			{
				JumpCurrentCount++;
			}

			bool bDidJump;
			
			if (JumpCurrentCount > 0 && !bWasJumping && !Cast<UThiefMovementComponent>(GetCharacterMovement())->IsInBurstMode())
			{
				bDidJump = false;
			}
			else
			{
				bDidJump = CanJump() && GetCharacterMovement()->DoJump(bClientUpdating);
			}
			
			if (bDidJump)
			{
				// Transition from not (actively) jumping to jumping.
				if (!bWasJumping)
				{
					JumpCurrentCount++;
					JumpForceTimeRemaining = GetJumpMaxHoldTime();
					OnJumped();
				}
			}

			bWasJumping = bDidJump;
		}
	}
}

void AThiefCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	
}

void AThiefCharacter::TraceLoc()
{
	endIndex = !endIndex;
	Cast<APlayerController>(GetController())->ClientMessage(TEXT("Switched"));
}

void AThiefCharacter::OnReceiveHint(const FString &Hint, float Time)
{
	UE_LOG(LogTemp, Warning, TEXT("Character received hint"));
	//Cast<APlayerController>(GetController())->GetHUD();

	OnHint.ExecuteIfBound(Hint, Time);
}

void AThiefCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
	if (AxisValue > 0)
	{
		bForwardBindDown = true;
	}
	else
	{
		bForwardBindDown = false;
	}
}

void AThiefCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AThiefCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AThiefCharacter::LookRightRate(float AxisValue)
{
	//UE_LOG(LogTemp, Display, TEXT("LOOKING YAW %f"), AxisValue);
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AThiefCharacter::Turn(float AxisValue) 
{
	//UE_LOG(LogTemp, Display, TEXT("LOOKING YAW %f"), AxisValue);
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AThiefCharacter::JumpPressed() 
{
	bJumpBindDown = true;
	FHitResult Hit;
	FHitResult NotHit;
	FVector InputVector = GetCharacterMovement()->GetLastInputVector();
	/* 
	if (InputVector == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Display, TEXT("Zero Input Vector"));
	}
	else if (InputVector == GetActorForwardVector())
	{
		UE_LOG(LogTemp, Display, TEXT("Same Vector!"));
	}
	 */
	// Mantle if possible
	if (AttemptMantle())
	{
		return;
	}
	else if (!GetCharacterMovement()->CanAttemptJump(), JumpRayTrace(Hit, -GetActorUpVector() * 50, true, InputVector))
	{
		//Walljump
		if(GEngine)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, TEXT("Walljumping..."), true, FVector2D(2, 2));
        }
		Cast<UThiefMovementComponent>(GetCharacterMovement())->Walljump(Hit.Normal);
	}
	else
	{
		//Start normal jump
		if (GetCharacterMovement()->CanAttemptJump())
		{

			
			
		}
		Jump();
	}
	/* 
	if (Hit.GetActor() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TRACE HIT %s"), *Hit.GetActor()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TRACE NO HIT"));
	}
	 */
}

void AThiefCharacter::JumpReleased() 
{
	bJumpBindDown = false;
	SetMantling(false);
	StopJumping();
}

void AThiefCharacter::SetMantling(bool Value) 
{
	bMantling = Value;
	if (Value)
	{
		// Stop character from moving so much whilst mantling
		FVector Velocity = GetCharacterMovement()->GetLastUpdateVelocity();
		float CounterZ = FMath::Min(Velocity.Z, 0.0f);
		FVector CounterVector = FVector(Velocity.X, Velocity.Y, CounterZ);
		GetCharacterMovement()->AddImpulse(-CounterVector, true);
		GetCharacterMovement()->GravityScale = 0;

		if (MSound && bMakeMantleNoise)
			{
				UGameplayStatics::SpawnSoundAttached(MSound, GetCamera());
				GetWorldTimerManager().SetTimer(TimerHandle_MantleNoiseCooldown, this, &AThiefCharacter::MantleNoiseCooldown, 0.2f, false, 0.2f);

				bMakeMantleNoise = false;
			}
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1;
	}
}

bool AThiefCharacter::JumpRayTrace(FHitResult& Hit, FVector Addon, bool Backwards, FVector InputVector) 
{
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	Params.bIgnoreTouches = true;
	
	FVector StartPoint = GetActorLocation() + Addon;
	int Multi = 80;
	if (Backwards) Multi = -Multi;

	/* UE_LOG(LogTemp, Display, TEXT("Input %s"), *InputVector.ToCompactString());
	UE_LOG(LogTemp, Display, TEXT("Forward %s"), *GetActorForwardVector().ToCompactString()); */
	/* if (InputVector == GetActorForwardVector())
	{
		UE_LOG(LogTemp, Display, TEXT("Same Vector!"));
	} */


	FVector EndPoint1 = StartPoint + GetActorForwardVector() * Multi;
	//UE_LOG(LogTemp, Display, TEXT("EndPoint Old %s"), *EndPoint.ToCompactString());
	//UE_LOG(LogTemp, Display, TEXT("EndPoint Alt %s"), *(StartPoint + InputVector * Multi).ToCompactString());
	FVector EndPoint2 = StartPoint + InputVector * Multi;
	/* 
	if (EndPoint1 == EndPoint2)
	{
		UE_LOG(LogTemp, Display, TEXT("SAame endpoint!"));
	}
	 */
	//DrawDebugSphere(GetWorld(), EndPoint1, 10.f, 4, FColor::Red, true, 2.f);
	//DrawDebugSphere(GetWorld(), EndPoint2, 10.f, 4, FColor::Green, true, 2.f);
	//DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Yellow, true, 2.f);
	FVector EndPoint;
	if (!endIndex)
	{
		EndPoint = EndPoint2;
	}
	else
	{
		EndPoint = EndPoint1;
	}
	return GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, EndPoint, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AThiefCharacter::Walljump(FVector Normal) 
{
	UE_LOG(LogTemp, Display, TEXT("Walljump level %i"), walljumpLevel);
	if (GetUpgradeComp()->walljumpLevel == 0) return;

	// Stop multiple wall jumps being performed off a single wall in an instant
	if (!bCanWalljump) return;
	bCanWalljump = false;

	FVector VelocityIntoWall = GetCharacterMovement()->GetLastUpdateVelocity() * FVector::DotProduct(GetCharacterMovement()->GetLastUpdateVelocity().GetSafeNormal(), Normal) - FVector::UpVector * FGenericPlatformMath::Min(0.0f, GetCharacterMovement()->GetLastUpdateVelocity().Z);
	FVector Location; FRotator Rotation;
	
	GetController()->GetPlayerViewPoint(Location, Rotation);
	float Pitch = Rotation.Pitch;

	FVector LookModifier;

	if (GetCharacterMovement()->GetLastInputVector() == GetActorForwardVector())
	{
		LookModifier = Rotation.Vector() * 350;
	}
	else LookModifier = GetCharacterMovement()->GetLastInputVector() * 350;

	GetCharacterMovement()->AddImpulse(VelocityIntoWall + FVector::UpVector * 600 + Rotation.Vector() * 350 + Normal * 100, true);
	if (WJSound != nullptr)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WJSound, GetActorLocation());
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_WalljumpCooldown, this, &AThiefCharacter::WalljumpCooldown, 0.2f, false, 0.2f);

}

bool AThiefCharacter::AttemptMantle() 
{
	if (GetUpgradeComp()->mantleLevel == 0)
	{ 
		//GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, TEXT("Mantle unavailable"), true, FVector2D(2, 2));
		return false;
	}

	FHitResult Hit;
	FHitResult NotHit;
	if (bForwardBindDown &&  JumpRayTrace(Hit, -GetActorUpVector() * 50, false, GetActorForwardVector()) && !JumpRayTrace(NotHit, GetActorUpVector() * 100, false, GetActorForwardVector()))
	{
		if (Hit.GetActor() == nullptr) return false;
		if (Hit.GetActor()->ActorHasTag("Orb"))return false;

		//Start Mantling
		//UE_LOG(LogTemp, Warning, TEXT("YES RAYTRACE"));
		if(GEngine)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, TEXT("Mantling..."), true, FVector2D(2, 2));
        }
		SetMantling(true);
		return true;
	}
	return false;
}