// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"
#include "SoundStimuli.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	//MovementComponent->SetupAttachment(RootComponent);
	MovementComponent->InitialSpeed = InitVelocity;
	MovementComponent->MaxSpeed = InitVelocity * 1.2f;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	GetWorldTimerManager().SetTimer(TimerHandle_Collidable, this, &AProjectile::BecomeCollidable, 0.05f, false, 0.05f);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ONHIT"));
	UE_LOG(LogTemp, Display, TEXT("HitComp %s"), *HitComp->GetName());
	UE_LOG(LogTemp, Display, TEXT("OtherActor %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Display, TEXT("OtherComp %s"), *OtherComp->GetName());

	auto MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}
	if (OtherActor == MyOwner) return;

	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();


	if (ExplosiveRadius == 0)
	{
		if(OtherActor && OtherActor != this && OtherActor != MyOwner)
		{
			/* if (HitCameraShakeClass && Cast<ATank>(OtherActor))
			{
				GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HitCameraShakeClass);
			} */

			UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, MyOwner, DamageTypeClass);
			
			
		}
	}
	else
	{
		UGameplayStatics::ApplyRadialDamageWithFalloff
		(
			this,
			Damage,
			Damage * 0.1f,
			GetActorLocation(),
			ExplosiveRadius * 0.2f + 20.f,
			ExplosiveRadius,
			1.f,
			DamageTypeClass,
			{},
			MyOwner,
			MyOwnerInstigator
		);
		
	}
	
	if (HitSound) {UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());}
	if (HitParticles) {UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());}
	USoundStimuli::CreateNoise(GetActorLocation(), AudibleRadius, 2, GetOwner(), "Explosion");

	Destroy();
	
}

void AProjectile::BecomeCollidable()
{
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

