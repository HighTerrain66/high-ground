// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundStimuli.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "ShooterCharacter.h"
#include "ShooterAIController.h"


USoundStimuli::USoundStimuli()
{
}

USoundStimuli::~USoundStimuli()
{
}

void USoundStimuli::CreateNoise(FVector Location, float Radius, uint8 Priority, class AActor* SourceActor, FString Reason) 
{
    FCollisionShape CollisionSphere;
	CollisionSphere.ShapeType = ECollisionShape::Sphere;
	CollisionSphere.SetSphere(Radius);
	
	//UE_LOG(LogTemp, Display, TEXT("NOISE CREATED %s"), *CollisionSphere.GetExtent().ToCompactString());
	TArray<struct FOverlapResult> OverlapResults;
	SourceActor->GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Location,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		CollisionSphere,
		FCollisionQueryParams::DefaultQueryParam
	);
	

	//DrawDebugSphere(GetWorld(), Location, Radius, 20, FColor::Red, false, 2, 0, 2);

	for (FOverlapResult Result : OverlapResults)
	{
		//UE_LOG(LogTemp, Display, TEXT("%s"), *Result.GetActor()->GetName());
		AShooterCharacter* PotentialCharacter = Cast<AShooterCharacter>(Result.GetActor());
		if (PotentialCharacter !=  
		+SourceActor)
		{
			//UE_LOG(LogTemp, Display, TEXT("NOISE CHARACTER OVERLAP"))
			AShooterAIController* PotentialController = Cast<AShooterAIController>(PotentialCharacter->GetController());
			if (PotentialController)
			{
				//UE_LOG(LogTemp, Display, TEXT("REGISTERING NOISE"))
				PotentialController->RegisterNoise(Location, Priority, SourceActor, Reason);
			}
		}
	}
}

