// Fill out your copyright notice in the Description page of Project Settings.


#include "ApproachingStorm.h"
#include "Components/AudioComponent.h"
#include "Sound/AmbientSound.h"
#include "NiagaraComponent.h"

// Sets default values
AApproachingStorm::AApproachingStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	BaseMesh->SetupAttachment(RootComponent);

	//RootComponent = BaseMesh;

	AudioSource = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Source"));

	ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle System"));
	ParticleSystem->SetupAttachment(RootComponent);
}

void AApproachingStorm::TimelineProgress(float Value)
{
	SetActorScale3D(FVector(1, BaseYScale * Value, BaseYScale));



	//UE_LOG(LogTemp, Display, TEXT("TIMELINE Y SCALE %f x %f"), BaseYScale, Value);
}

// Called when the game starts or when spawned
void AApproachingStorm::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	BaseYScale = GetActorScale().Y;

	//UE_LOG(LogTemp, Display, TEXT("BASEYSCALE %f"), BaseYScale);

	SetActorScale3D(FVector(GetActorScale3D().X, 0, GetActorScale3D().Z));
	// Bind timeline function
    if (CurveFloat)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
        SizeTimeline.AddInterpFloat(CurveFloat, TimelineProgress);

		//SizeTimeline.PlayFromStart();
    }

	ParticleSystem->DeactivateImmediate();



	StartLocation = GetActorLocation();
}

void AApproachingStorm::Reset(bool go)
{
	bStormApproached = false;

	SetActorLocation(StartLocation);
	SetActorScale3D(FVector(GetActorScale3D().X, 0, GetActorScale3D().Z));

	SizeTimeline.Stop();

	if (go)
	{
		BaseVelocity = 600.f;
		Acceleration = 80.f;
		SizeTimeline.PlayFromStart();
	}
	else
	{
		BaseVelocity = 0.f;
		Acceleration = 0.f;
	}

	SkyShift(go);

	TotalDeltaTime = 0.f;
	Counts = 1.f;
	

	if (MusicSource)
		MusicSource->AdjustVolume(0.01f, 1.f);

		UE_LOG(LogTemp, Display, TEXT("RESET-   BASE %f"), BaseVelocity);

	ParticleSystem->ActivateSystem();
}

// Called every frame
void AApproachingStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerActor == nullptr) return;

	TotalDeltaTime += DeltaTime;

	float Velocity = BaseVelocity;
	float XDistance = FMath::Abs(PlayerActor->GetActorLocation().X - GetActorLocation().X);

	if (XDistance > 35000 && BaseVelocity > 0)
	{
		Velocity += 1800;
	}
	if (XDistance > 22000 && BaseVelocity > 0)
	{
		Velocity += 1400;

		if (MusicSource)
		MusicSource->AdjustVolume(1.f, 1.f);
	}
	else if (!bStormApproached && XDistance < 6000)
	{
		bStormApproached = true;
		AudioSource->Play();
	}
	else if (bStormApproached && XDistance < 5000)
	{
		if (MusicSource)
		MusicSource->AdjustVolume(0.5f, (XDistance - 1500) / 5000.f);
	}
	else
	{
		if (MusicSource)
		{
			MusicSource->AdjustVolume(1.f, 1.f);

			if (!MusicSource->GetAudioComponent()->IsPlaying())
			MusicSource->Play();
		}
		
	}
	AddActorWorldOffset(-FVector::ForwardVector * Velocity * DeltaTime, false);


	// Acceleration

	if (TotalDeltaTime > 10 * Counts)
	{
		if (Counts < 6)
		{
			BaseVelocity += Acceleration;
		}
		else
		{
			BaseVelocity += Acceleration * 0.4f;
		}
		
		Counts += 1;

		UE_LOG(LogTemp, Display, TEXT("Speed up-   BASE %f,  CURRENT %f"), BaseVelocity, Velocity);
	}

	// Expansion

	if (GetActorLocation().X < -24957.0f)
	{
		SetActorScale3D(GetActorScale3D() + FVector::RightVector * 2.f * DeltaTime);
	}

	SizeTimeline.TickTimeline(DeltaTime);
}

