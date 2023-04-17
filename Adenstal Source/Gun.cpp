// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Components\CapsuleComponent.h"
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"
#include "ShooterAIController.h"
#include "SoundStimuli.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetGenerateOverlapEvents(true);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	GunMesh->SetupAttachment(Root);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn"));
	ProjectileSpawnPoint->SetupAttachment(GunMesh);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsOnGround)
	{
		//shorthand for enabling collision
		SetIsOnGround(true);
	}
	else
	{
		SetIsOnGround(false);
	}

	Root->OnComponentBeginOverlap.AddDynamic(this, &AGun::OnOverlap);
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Load new rounds into chamber
	if (Load < ShotSeparation)
	{
		//Load += FMath::Min(DeltaTime, ShotSeparation - Load);
		Load += DeltaTime;
		if (Load > ShotSeparation) {Load = ShotSeparation;}
	}
	if (Load2 < ShotSeparation2)
	{
		Load2 += FMath::Min(DeltaTime, ShotSeparation2 - Load2);
	}

	// Automatic firing
	if (SustainedFire && Load == ShotSeparation && Ammo > 0)
	{
		
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMesh, TEXT("MuzzleFlashSocket"));

		//DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 1.f, FColor::Purple, true);
		if (MuzzleVelocity == 0)
		{
			HitScan(BaseDamage, MaxRange, ImpactSound, ImpactEffect);
		}
		else
		{
			Projectile(BaseDamage, MaxRange, ProjectileClass);
		}
		Ammo -=1;
		Load = 0;
	}

	if (CurrentRecoil >= BaseRecoil2 * SubsequentShotRecoilModifier)
	{
		CurrentRecoil = BaseRecoil1 * SubsequentShotRecoilModifier * DeltaTime;
	}
	else if (CurrentRecoil > BaseRecoil1)
	{
		CurrentRecoil -= DeltaTime * 0.5f;
		CurrentHorizontalRecoil -= DeltaTime * (CurrentHorizontalRecoil / FMath::Abs(CurrentHorizontalRecoil * 0.4f)) * 0.3f;
	}
	else
	{
		CurrentRecoil = BaseRecoil1;
		CurrentHorizontalRecoil = BaseHorizontalRecoil;
	}

	if (FMath::Abs(CurrentHorizontalRecoil) > MaxHorizontalRecoil) { CurrentHorizontalRecoil /= SubsequentShotRecoilModifier * 2;}
}

int32 AGun::GetAmmo1() const
{
	//UE_LOG(LogTemp, Display, TEXT("AMMO  %i"), Ammo);
	return Ammo;
}
int32 AGun::GetAmmo2() const
{
	//UE_LOG(LogTemp, Display, TEXT("AMMO2 %i"), Ammo2);
	return Ammo2;
}

void AGun::SetIsOnGround(bool Value) 
{
	IsOnGround = Value;
	if (Value)
	{
		Root->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void AGun::PullTrigger(float Spread)
{
	//UE_LOG(LogTemp, Warning, TEXT("Trigger pulled!"));
	if (AutoFire)
	{
		SustainedFire = true;
	}
	else if (Load == ShotSeparation && Ammo > 0)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"), FVector::ZeroVector, FRotator::ZeroRotator, FVector(1, 1, 1), EAttachLocation::KeepRelativeOffset);
		//UGameplayStatics::SpawnSoundAttached(MuzzleSound, Cast<ACharacter>(GetOwner())->GetMesh());
		Cast<AShooterCharacter>(GetOwner())->ReportNoise(MuzzleSound, 1.f);
		CreateNoise(GetOwner()->GetActorLocation(), 50000.f, 2, GetOwnerController());

		//DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 1.f, FColor::Purple, true);
		if (MuzzleVelocity == 0)
		{
			HitScan(BaseDamage, MaxRange, ImpactSound, ImpactEffect, Spread);
		}
		else
		{
			Projectile(BaseDamage, MaxRange, ProjectileClass);
		}
		Ammo -= 1;
		Load = 0;
	}
	
	
	
}

void AGun::ReleaseTrigger() 
{
	SustainedFire = false;
}


void AGun::PullTrigger2() 
{
	CurrentRecoil = BaseRecoil2;
	//UE_LOG(LogTemp, Warning, TEXT("Trigger2 pulled!"));
	if (Load2 == ShotSeparation2 && Ammo2 > 0)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash2, GunMesh, TEXT("MuzzleFlashSocket"));
		Cast<AShooterCharacter>(GetOwner())->ReportNoise(MuzzleSound2, 1.f);
		CreateNoise(GetActorLocation(), 5000.f, 2, GetOwnerController());

		//DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 1.f, FColor::Purple, true);
		if (MuzzleVelocity2 == 0)
		{
			HitScan(BaseDamage, MaxRange, ImpactSound, ImpactEffect);
		}
		else
		{
			Projectile(BaseDamage2, MaxRange2, ProjectileClass2);
		}
		Ammo2 -=1;
		Load2 = 0;
	}
	
}

void AGun::HitScan(float Damage, float Range, class USoundBase* Sound, UParticleSystem* Effect, float Spread) 
{
	
	FHitResult Hit;
	FVector ShotDirection;
	if (GunTrace(Hit, ShotDirection, Spread))
	{
		if (Hit.GetActor())
		{
			//UE_LOG(LogTemp, Display, TEXT("Hit object %s"), *Hit.GetActor()->GetName());
			FPointDamageEvent DamageEvent(BaseDamage, Hit, ShotDirection, nullptr);
			Hit.GetActor()->TakeDamage(BaseDamage, DamageEvent, GetOwnerController(), GetOwner());
		}
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Orange, true);
		if (ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		}
		if (ImpactSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, Hit.Location, ShotDirection.Rotation());
			USoundStimuli::CreateNoise(GetActorLocation(), 5000.f, 2, GetOwner());
			USoundStimuli::CreateNoise(GetActorLocation(), 3000.f, 1, GetOwner());
			//CreateNoise(GetActorLocation(), 5000.f, 2, GetOwnerController());
			//CreateNoise(Hit.Location, 2000.f, 1, GetOwnerController());
		}
		
	}
	Recoil();
}

void AGun::Projectile(float Damage, float Range, TSubclassOf<AProjectile> Class) 
{
	FVector NotNeeded;
	FRotator Path;
	GetOwnerController()->GetPlayerViewPoint(NotNeeded, Path);
	Path += FRotator(0.f, 3.f, 0.f);

	auto Projectile = GetWorld()->SpawnActor<AProjectile>(Class, ProjectileSpawnPoint->GetComponentLocation(), Path);
	Projectile->SetOwner(this);
	Projectile->Damage = Damage;
	Projectile->SetActorScale3D(Projectile->GetActorScale3D() * 0.5f);

	Recoil();
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection, float Spread) 
{
	
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) {return false;}
	
	FVector ViewpointLocation;
	FRotator ViewpointRotation;

	OwnerController->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);
	
	FRotator SpreadRotator = FRotator::ZeroRotator;
	if (Spread > 0)
	{
		int32 Yaw = FMath::RandHelper(90);
		int32 Pitch = FMath::RandHelper(90);
		SpreadRotator = FRotator(Pitch, Yaw, 0) * Spread;
	}

	ShotDirection = -(ViewpointRotation + SpreadRotator).Vector();
	//DrawDebugCamera(GetWorld(), ViewpointLocation, ViewpointRotation, 90, 1.f, FColor::Green, true);

	FVector Endpoint = ViewpointLocation + ViewpointRotation.Vector() * MaxRange;

	//FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, ViewpointLocation, Endpoint, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AGun::Recoil() 
{
	CurrentRecoil = CurrentRecoil * SubsequentShotRecoilModifier;
	CurrentHorizontalRecoil = CurrentHorizontalRecoil * SubsequentShotRecoilModifier * 1.65f;
	AShooterCharacter* Shooter = Cast<AShooterCharacter>(GetOwner());
	if (Shooter == nullptr) {return;}
	Shooter->RecoilInputVertical += CurrentRecoil;

	int Rand = FMath::RandRange(0, 6);
	float HorizontalRecoilInput = -CurrentHorizontalRecoil + (Rand/6) * CurrentHorizontalRecoil * 1.05f;
	//UE_LOG(LogTemp, Display, TEXT("HorizontalRecoil: %f"), HorizontalRecoilInput);
	Shooter->RecoilInputHorizontal += HorizontalRecoilInput;
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	AController* OwnerController = OwnerPawn->GetController();
	return OwnerController;
}

void AGun::OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	UE_LOG(LogTemp, Display, TEXT("GUN PICKUP?"));
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	AShooterCharacter* OtherPawn = Cast<AShooterCharacter>(OtherActor);
	if (!OtherPawn) return;

	if (Cast<AShooterPlayerController>(OtherPawn->Controller))
	{
		AGun* OtherGun = OtherPawn->GetGun();
		if (OtherGun == nullptr) return;
		if (OtherGun->GetGunID() == GunID)
		{
			if (PickupSound)
			{
				UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator);
			}
			
			OtherGun->Ammo += PickupAmmo1;
			OtherGun->Ammo2 += PickupAmmo2;
			Destroy();
		}
	}
}

void AGun::CreateNoise(FVector Location, float Radius, uint8 Priority, AController* SourceController) 
{
	FCollisionShape CollisionSphere;
	CollisionSphere.ShapeType = ECollisionShape::Sphere;
	CollisionSphere.SetSphere(Radius);
	
	UE_LOG(LogTemp, Display, TEXT("NOISE CREATED %s"), *CollisionSphere.GetExtent().ToCompactString());
	TArray<struct FOverlapResult> OverlapResults;
	GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Location,
		GetActorRotation().Quaternion(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		CollisionSphere,
		FCollisionQueryParams::DefaultQueryParam
	);

	//DrawDebugSphere(GetWorld(), Location, Radius, 20, FColor::Red, false, 2, 0, 2);

	for (FOverlapResult Result : OverlapResults)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *Result.GetActor()->GetName());
		AShooterCharacter* PotentialCharacter = Cast<AShooterCharacter>(Result.GetActor());
		if (PotentialCharacter)
		{
			UE_LOG(LogTemp, Display, TEXT("NOISE CHARACTER OVERLAP"))
			AShooterAIController* PotentialController = Cast<AShooterAIController>(PotentialCharacter->GetController());
			if (PotentialController)
			{
				UE_LOG(LogTemp, Display, TEXT("REGISTERING NOISE"))
				PotentialController->RegisterNoise(Location, Priority);
			}
		}
	}


}

