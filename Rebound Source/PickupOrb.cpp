// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupOrb.h"
#include "Components/SphereComponent.h"
#include "ThiefCharacter.h"
#include "UpgradeHandleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ReboundComponent.h"

// Sets default values
APickupOrb::APickupOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = Collider;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void APickupOrb::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APickupOrb::OnOverlapBegin);
}

void APickupOrb::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, TEXT("=points"), true, FVector2D(2, 2));
        } */
	// Add points to player
	AThiefCharacter* PlayerCharacter = Cast<AThiefCharacter>(OtherActor);
	
	if (PlayerCharacter == nullptr) return;

	// Don't allow players to collect orbs whilst in a rebound animation
	if (PlayerCharacter->GetReboundComp()->IsRebounding())
	return; 

	UUpgradeHandleComponent* UpgradeHandle = PlayerCharacter->GetUpgradeComp();

	if (UpgradeHandle == nullptr) return;

	UpgradeHandle->AddOrbs(Value);

	if (PickupSFX) UGameplayStatics::SpawnSoundAtLocation(this, PickupSFX, GetActorLocation());
	/* if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, TEXT("Added points"), true, FVector2D(2, 2));
        } */

	// Then destroy this actor
	Destroy();
}

// Called every frame
void APickupOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
