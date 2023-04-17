// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ThiefCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UThiefMovementComponent::UThiefMovementComponent()
{
    
}



void UThiefMovementComponent::BeginPlay() 
{
    Super::BeginPlay();

    AThiefCharacter* Owner = Cast<AThiefCharacter>(GetOwner());

    // Assigning movement attributes to what are effectively constants
    StandingCapsuleHalfHeight = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    StandingCameraZOffset = Owner->GetCamera()->GetRelativeLocation().Z;
    StandingBrakingFrictionFactor = BrakingFrictionFactor;
    CrouchingBrakingFrictionFactor = BrakingFrictionFactor / 20;

    StandingFriction = GroundFriction;
    SlidingFriction = GroundFriction / 2;

    WalkingAcceleration = MaxAcceleration;

    if (CurveFloat)
    {
        
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
        SlideTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
    }
}

void UThiefMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) 
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (MovementState) {
        case Walking:
            if (IsMovingOnGround())
            {
                StepProgress += DeltaTime * FMath::Sqrt(GetLastUpdateVelocity().Size());
                if (StepProgress > 10.f)
                {
                    USoundBase* Footstep = FootstepSFX[0];
                    
                    FHitResult FloorResult = CurrentFloor.HitResult;

                    

                    if (FloorResult.PhysMaterial.IsValid())
                    switch (FloorResult.PhysMaterial.Get()->SurfaceType)
                    {
                        case EPhysicalSurface::SurfaceType_Default:
                        UE_LOG(LogTemp, Display, TEXT("Normal"));
                            break;
                        case EPhysicalSurface::SurfaceType1:
                        UE_LOG(LogTemp, Display, TEXT("Wa'er"));
                            Footstep = WaterFootstepSFX[0];
                            break;
                        default:
                            break;
                    }
                    else
                    {
                        UE_LOG(LogTemp, Display, TEXT("No mat"));
                    }

                    UGameplayStatics::SpawnSoundAtLocation(this, Footstep, GetActorLocation());
                    StepProgress -= 10.f; 


                }
            }
            
            break;
        case Sliding:
            if (CanStand()) ResolveMovementState();// Stand up if key is released and there is room to stand
            else
            {
                // Make character slide along floor gradient
                FVector FloorNormal = CurrentFloor.HitResult.Normal;
                AddForce(CalculateFloorInfluence(FloorNormal) * 100000);
            }

            // play slide sfx if the slide was started mid air
            if (bSlideSFXQueued && MovementMode != EMovementMode::MOVE_Falling && SlideSound != nullptr)
            {
                FVector FloorNormal = CurrentFloor.HitResult.Normal;
                AddForce(CalculateFloorInfluence(FloorNormal) * 600000);

                UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SlideSound, GetActorLocation() - FVector::UpVector * CurrentHalfHeight);
                bSlideSFXQueued = false;
            }
            break;
    }

    // Handles
    SlideTimeline.TickTimeline(DeltaTime);

}

void UThiefMovementComponent::TimelineProgress(float Value)
{
    FVector NewCamOffset = FVector(0.0f, 0.0f, FMath::Lerp(StandingCameraZOffset, 25.0f, Value));
    Cast<AThiefCharacter>(GetOwner())->GetCamera()->SetRelativeLocation(NewCamOffset);


    float NewCapsuleHalfHeight = FMath::Lerp(StandingCapsuleHalfHeight, 25.0f, Value);
    Cast<AThiefCharacter>(GetOwner())->GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHalfHeight);
}



void UThiefMovementComponent::ResolveMovementState() 
{
    TEnumAsByte<EMovementState> NewState;

    if (CanStand()) NewState = EMovementState::Walking;

    else NewState = EMovementState::Sliding;

    SetMovementState(NewState);
}

void UThiefMovementComponent::SetMovementState(TEnumAsByte<EMovementState> NewState) 
{
    if (NewState == MovementState) return;

    TimeSinceStateChange = 0.f;
    
    switch (MovementState) {
        case Walking:
            if (NewState == Sliding)
            {
                BeginSlide();
            }
            else
            {
                BeginDash();
            }
            
            break;
        case Sliding:
            EndSlide();
            break;
        case Dashing:
            //EndDash();
            break;
    }

    MovementState = NewState;
}

FVector UThiefMovementComponent::CalculateFloorInfluence(FVector FloorNormal) 
{
    if (FloorNormal.Equals(FVector::ZeroVector, 0.0001f))
    {
        return FVector::ZeroVector;
    }
    else
    {
        return FVector::CrossProduct(FloorNormal, FVector::CrossProduct(FloorNormal, FVector::UpVector)).GetUnsafeNormal();
    }

}



bool UThiefMovementComponent::CanSlide()
{
    return true;
}

bool UThiefMovementComponent::CanStand() 
{
    if (bSlideKeyDown)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool UThiefMovementComponent::DoJump(bool bReplayingMoves) 
{
    if ( CharacterOwner && CharacterOwner->CanJump() )
	{
		// Don't jump if we can't move up/down.
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
            if (MovementMode != MOVE_Falling)
            {
                
            }
            

            if (JSound != nullptr && MovementMode != MOVE_Falling)
            {
                UGameplayStatics::SpawnSoundAtLocation(GetWorld(), JSound, GetActorLocation());
            }
            else if (AirHikeSound != nullptr && CharacterOwner->JumpKeyHoldTime == 0)
            {
                UGameplayStatics::SpawnSoundAtLocation(GetWorld(), AirHikeSound, GetActorLocation());
            }
        }
    }
    
    bool result = Super::DoJump(bReplayingMoves);

    if (result)
    {
        AddImpulse(GetLastInputVector() * JumpBoost, true);
    }

    return result;

}



void UThiefMovementComponent::BeginSlide() 
{
    BrakingFrictionFactor = CrouchingBrakingFrictionFactor;
    GroundFriction = SlidingFriction;
    MaxAcceleration = 100;
    BrakingDecelerationWalking = 0;
    
    if(GEngine)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Sliding..."), true, FVector2D(2, 2));
        }
    SlideTimeline.Play();
    if (SlideSound != nullptr && MovementMode != MOVE_Falling)
    {
        AddImpulse(GetOwner()->GetActorForwardVector() * 40000.0f);
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SlideSound, GetActorLocation());

        FVector FloorNormal = CurrentFloor.HitResult.Normal;
                AddForce(CalculateFloorInfluence(FloorNormal) * 600000);
    }
    else
    {
        bSlideSFXQueued = true;
    }
}

void UThiefMovementComponent::EndSlide() 
{
    BrakingFrictionFactor = StandingBrakingFrictionFactor;
    GroundFriction = StandingFriction;
    MaxAcceleration = WalkingAcceleration;
    BrakingDecelerationWalking = 50;
    if(GEngine)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Stopped..."), true, FVector2D(2, 2));
        }
    SlideTimeline.Reverse();

    bSlideAvailable = false;
    GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_SlideCooldown, this, &UThiefMovementComponent::SlideCooldown, 0.4f, false, 0.4f);
}

void UThiefMovementComponent::SlidePressed()
{
    if(GEngine)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Pressed"), true, FVector2D(2, 2));
    }

    if (bBurstKeyDown)
    {
        // Do Plunge stuff
        /* if (bDashAvailable)
        {
            SetMovementState(EMovementState::Dashing);
        } */
        Plunge();
    }


    bSlideKeyDown = true;
    if (bSlideAvailable && SlideLevel > 0)
    {
        SetMovementState(EMovementState::Sliding);
    }
    
}

void UThiefMovementComponent::SlideReleased()
{
    if(GEngine)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Released"), true, FVector2D(2, 2));
        }
    bSlideKeyDown = false;
    SetMovementState(EMovementState::Walking);
}

void UThiefMovementComponent::DashPressed()
{

    // Do Dash stuff
    if (bDashAvailable && DashLevel > 0)
    {
        SetMovementState(EMovementState::Dashing);
        if (DashLevel > 2)
        {
            bSecondDashAvailable = true;
        }
    }
    else if (bSecondDashAvailable)
    {
        SetMovementState(EMovementState::Dashing);
        bSecondDashAvailable = false;
    }
}

void UThiefMovementComponent::DashReleased()
{
    EndDash();
}

void UThiefMovementComponent::SlideCooldown()
{
    bSlideAvailable = true;
    if (bSlideKeyDown)
    {
        SetMovementState(EMovementState::Sliding);
    }
}

void UThiefMovementComponent::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
    Super::ProcessLanded(Hit, remainingTime, Iterations);

    StepProgress = 0.f;
    if (LandedSFX[0] == nullptr) return;

    USoundBase* LandedSound = LandedSFX[0];
                    
    FHitResult FloorResult = CurrentFloor.HitResult;

    

    if (FloorResult.PhysMaterial.IsValid())
    switch (FloorResult.PhysMaterial.Get()->SurfaceType)
    {
        case EPhysicalSurface::SurfaceType_Default:
        UE_LOG(LogTemp, Display, TEXT("Normal"));
            break;
        case EPhysicalSurface::SurfaceType1:
        UE_LOG(LogTemp, Display, TEXT("Wa'er"));
            LandedSound = WaterLandedSFX[0];
            break;
        default:
            break;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("No mat"));
    }

    if (bPlayMovementSounds)
    UGameplayStatics::SpawnSoundAtLocation(this, LandedSound, GetActorLocation() - FVector::UpVector * CurrentHalfHeight);
    //UGameplayStatics::SpawnSoundAtLocation(this, LandedSFX[0], GetActorLocation() - FVector::UpVector * CurrentHalfHeight);
    bDashAvailable = true;
}

void UThiefMovementComponent::Walljump(FVector Normal)
{
    UE_LOG(LogTemp, Display, TEXT("Walljump level %i"), WalljumpLevel);
	if (WalljumpLevel == 0) return;

    if (DashLevel > 1)
    {
        bDashAvailable = true;
    }
    

	// Stop multiple wall jumps being performed off a single wall in an instant
	if (!bCanWalljump) return;
	bCanWalljump = false;

	FVector VelocityIntoWall = GetLastUpdateVelocity() * FVector::DotProduct(GetLastUpdateVelocity().GetSafeNormal(), Normal) - FVector::UpVector * FGenericPlatformMath::Min(0.0f, GetLastUpdateVelocity().Z);
	FVector Location; FRotator Rotation;
	
	GetCharacterOwner()->GetController()->GetPlayerViewPoint(Location, Rotation);
	float Pitch = Rotation.Pitch;

	FVector LookModifier;

	if (GetLastInputVector() == GetOwner()->GetActorForwardVector())
	{
		LookModifier = Rotation.Vector() * 350;
	}
	else LookModifier = GetLastInputVector() * 350;

	AddImpulse(VelocityIntoWall + FVector::UpVector * 600 + Rotation.Vector() * 350 + Normal * 100, true);
	if (WJSound != nullptr)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WJSound, GetOwner()->GetActorLocation());
	}
	
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_WalljumpCooldown, this, &UThiefMovementComponent::WalljumpCooldown, 0.2f, false, 0.2f);
}

void UThiefMovementComponent::BurstPressed()
{
    UE_LOG(LogTemp, Display, TEXT("DASH"));
    bBurstKeyDown = true;
}

void UThiefMovementComponent::BurstReleased()
{
    bBurstKeyDown = false;
}

void UThiefMovementComponent::BeginDash()
{
    UE_LOG(LogTemp, Display, TEXT("DASH"));
    GravityScale = 0.f;

    FVector Location; FRotator Rotation;
	
	Cast<ACharacter>(GetOwner())->GetController()->GetPlayerViewPoint(Location, Rotation);


    /* FTransform ViewTransform = FTransform(Rotation, Location, FVector(1,1,1));
    AddImpulse(Rotation.Vector() * 10000);

    FVector RightVelocity = ViewTransform.InverseTransformVector(Velocity).X;
    FVector UpVeclocity = ViewTransform.InverseTransformVector(Velocity).Y; */

    FTransform Transform = GetOwner()->GetActorTransform();

    float RightVelocity = Transform.InverseTransformVector(Velocity).Y;
    float UpVelocity = Transform.InverseTransformVector(Velocity).Z;
    float BackwardsVelocity = Transform.InverseTransformVector(Velocity).X;

    if (BackwardsVelocity > 0) BackwardsVelocity = 0;

    UE_LOG(LogTemp, Display, TEXT("Up %f"), UpVelocity);

    AddImpulse(-RightVelocity * GetOwner()->GetActorRightVector() - UpVelocity * GetOwner()->GetActorUpVector() - BackwardsVelocity * GetOwner()->GetActorForwardVector(), true);
    AddImpulse(GetOwner()->GetActorForwardVector() * 1500, true);
    //transform.InverseTransformDirection(playerBody.velocity).x > 0

    OnDash.ExecuteIfBound();

    GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_DashEnd, this, &UThiefMovementComponent::EndDash, 0.3f, false, 0.3f);
    bDashAvailable = false;

    if (DashSFX)
    {
        UGameplayStatics::SpawnSoundAttached(DashSFX, Cast<AThiefCharacter>(GetOwner())->GetMesh());
    }

}

void UThiefMovementComponent::EndDash()
{
    GravityScale = 1.f;
    //MovementMode = EMovementMode::MOVE_Walking;
    GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_DashEnd);
    SetMovementState(EMovementState::Walking);
    if (IsFalling() == false)
    {
        bDashAvailable = true;
    }
}

void UThiefMovementComponent::Plunge()
{
    Velocity = FVector(0, 0, -4000);

    if (DashSFX)
    {
        UGameplayStatics::SpawnSoundAttached(DashSFX, Cast<AThiefCharacter>(GetOwner())->GetMesh());
    }
}

bool UThiefMovementComponent::IsInBurstMode()
{
    return bBurstKeyDown;
}

