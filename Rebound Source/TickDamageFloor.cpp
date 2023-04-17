// Fill out your copyright notice in the Description page of Project Settings.


#include "TickDamageFloor.h"
#include "Components/BoxComponent.h"
#include "ThiefCharacter.h"

// Sets default values
ATickDamageFloor::ATickDamageFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = BaseMesh;

	DamageZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box"));
	DamageZone->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ATickDamageFloor::BeginPlay()
{
	Super::BeginPlay();
	
	DamageZone->OnComponentBeginOverlap.AddDynamic(this, &ATickDamageFloor::OnOverlapBegin);
	DamageZone->OnComponentEndOverlap.AddDynamic(this, &ATickDamageFloor::OnOverlapEnd);
}

// Called every frame
void ATickDamageFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ContactedCharacter != nullptr)
	{
		// Damage orb count by 2 for every 0.35s of contact
		DamageBuildup += DeltaTime;

		if (DamageBuildup > 0.35f)
		{
			ContactedCharacter->GetUpgradeComp()->AddOrbs(-2);
			DamageBuildup -= 0.35f;
		}
		
	}
}

void ATickDamageFloor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AThiefCharacter* trialCharacter = Cast<AThiefCharacter>(OtherActor);

	if (trialCharacter != nullptr)
	{
		ContactedCharacter = trialCharacter;

		ContactedCharacter->GetUpgradeComp()->AddOrbs(-5);
	}
}

void ATickDamageFloor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AThiefCharacter* trialCharacter = Cast<AThiefCharacter>(OtherActor);

	if (trialCharacter != nullptr)
	{
		ContactedCharacter = nullptr;
	}
}


