// Fill out your copyright notice in the Description page of Project Settings.


#include "ReboundComponent.h"
#include "ThiefCharacter.h"
#include "ThiefMovementComponent.h"
#include "EngineUtils.h"
#include "ApproachingStorm.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UReboundComponent::UReboundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UReboundComponent::BeginPlay()
{
	Super::BeginPlay();

	ReboundTarget = GetOwner()->GetActorLocation();

	PlayerCharacter = Cast<AThiefCharacter>(GetOwner());
	PlayerMovement = Cast<UThiefMovementComponent>(PlayerCharacter->GetMovementComponent());

	// Bind timeline function
    if (CurveFloat)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
        ReboundTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
    }

	for (TActorIterator<AApproachingStorm> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		HazardField = Cast<AApproachingStorm>(*ActorItr);
 	}

	
}


// Called every frame
void UReboundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ReboundTimeline.TickTimeline(DeltaTime);
}

void UReboundComponent::TimelineProgress(float Value)
{
	GetOwner()->SetActorLocation(FMath::Lerp(ReboundStartLocation, ReboundTarget, Value));
	if (Value >= 1)
	{
		// Finish rebound
		bIsRebounding = false;

		PlayerMovement->bPlayMovementSounds = true;

		ReboundTimeline.Stop();
	}
}

void UReboundComponent::StartRebound()
{
	//UE_LOG(LogTemp, Display, TEXT("componet rebound"));
	if (bIsRebounding) return;

	ReboundStartLocation = GetOwner()->GetActorLocation();
	ReboundTimeline.PlayFromStart();
	bIsRebounding = true;

	HazardField->Reset(false);

	if (ReboundSFX) UGameplayStatics::SpawnSoundAttached(ReboundSFX, PlayerCharacter->GetMesh());

	PlayerMovement->bPlayMovementSounds = false;
}

