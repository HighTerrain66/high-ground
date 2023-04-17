// Fill out your copyright notice in the Description page of Project Settings.


#include "ThiefCameraComponent.h"
#include "ThiefCharacter.h"
#include "ThiefMovementComponent.h"

#include "NiagaraComponent.h"

UThiefCameraComponent::UThiefCameraComponent()
{
    //RegisterComponent();
    PrimaryComponentTick.bCanEverTick = true;
    SetComponentTickEnabled(true);

    ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle System"));
	ParticleSystem->SetupAttachment(this);
    
    ParticleSystem->SetEmitterEnable(TEXT("DashBurst"), false);
}

void UThiefCameraComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) 
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    float TargetFOV;

    float currentSpeed = Movement->GetLastUpdateVelocity().Size();

    
    if (currentSpeed > 1800)
    {
        TargetFOV = MaxFOV;
    }
    else if (currentSpeed > 1050)
    {
        TargetFOV = FMath::Lerp(BaseFOV, MaxFOV, (1800 - currentSpeed) / 750.f);
    }
    else
    {
        TargetFOV = BaseFOV;
    }

    FieldOfView = FMath::Lerp(FieldOfView, TargetFOV, 0.01f);

    ParticleSystem->SetEmitterEnable(TEXT("DashBurst"), false);
}

void UThiefCameraComponent::BeginPlay()
{
    Super::BeginPlay();


    AThiefCharacter* ThiefOwner = Cast<AThiefCharacter>(GetOwner());
    Movement = Cast<UThiefMovementComponent>(ThiefOwner->GetCharacterMovement());

    Movement->OnDash.BindUObject(this, &UThiefCameraComponent::DashEffect);//Dynamic(this, &UThiefCameraComponent::DashEffect);

    ParticleSystem->SetAsset(SystemEffect);
    
}

void UThiefCameraComponent::DashEffect()
{
    FieldOfView += 4.f;

    //ParticleSystem->SetEmitterEnable(TEXT("DashBurst"), true);
    ParticleSystem->ActivateSystem();
    
}

void UThiefCameraComponent::DashEffectEnd()
{
    ParticleSystem->SetEmitterEnable(TEXT("DashBurst"), false);
}
