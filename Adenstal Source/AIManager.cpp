// Fill out your copyright notice in the Description page of Project Settings.


#include "AIManager.h"
#include "ShooterAIController.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Ambience/Music"));


}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (AmbientSound)
	{
		AudioComp->SetSound(AmbientSound);
		AudioComp->Play();
	}
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterAIController::StaticClass(), Subordinates);

	for (AActor* Grunt : Subordinates)
	{
		AShooterAIController* GruntController = Cast<AShooterAIController>(Grunt);
		GruntController->FirstAlert.AddDynamic(this, &AAIManager::ReceiveFirstAlert);
	}
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIManager::whosthere(bool Value) 
{
	for (AActor* Grunt : Subordinates)
	{
		AShooterAIController* Subordinate = Cast<AShooterAIController>(Grunt);
		Subordinate->bIsBlind = Value;
		UE_LOG(LogTemp, Display, TEXT("BLINDNESS"));
	}
}

void AAIManager::ReceiveFirstAlert() 
{
	UE_LOG(LogTemp, Display, TEXT("FIRST ALERT"));
	if (++GruntsAlerted > 1)
	{
		UE_LOG(LogTemp, Display, TEXT("ALERT"));
		// do the music thing
		if (AlertMusic && AudioComp->Sound != AlertMusic)
		{
			AudioComp->SetSound(AlertMusic);
			AudioComp->Play();
		}
	}
}

