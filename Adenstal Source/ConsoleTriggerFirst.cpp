// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsoleTriggerFirst.h"
#include "ShooterCharacter.h"
#include "ShooterAIController.h"

AConsoleTriggerFirst::AConsoleTriggerFirst() 
{
    //OnActorBeginOverlap.AddDynamic(this, &AConsoleTriggerFirst::OnOverlapBegin);
}


void AConsoleTriggerFirst::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) 
{
    Super::OnOverlapBegin(OverlappedActor, OtherActor);

    if (!Activated && OtherActor == ActorThatOpens)
    {
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("SECURITY LOCKDOWN OVERRIDDEN"));
        }
        UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN YES GREAT"));

        
        //GetWorld()->SpawnActor(ActorToSpawn, TEXT("QRF Guard 1"), FVector(4309.699219f,-5754.177246f,966.071106f), FRotator(0.f, -159.f, 0.f))
        FVector Location(4309.699219f,-5754.177246f,966.071106f);
        const FRotator Rotation(0.f, -159.f, 0.f);
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.Name = TEXT("QRF Grunt1");
        AShooterCharacter* Grunt1 =GetWorld()->SpawnActor<AShooterCharacter>(ClassToSpawn, Location, Rotation, SpawnInfo);
        AShooterAIController* Controller1 = Cast<AShooterAIController>(Grunt1->GetController());
        if (!Controller1) return;
        Controller1->BeginInvestigation(GetActorLocation());
        /* Cast<AAIController>(Grunt1->GetController())->GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), OtherActor);
        Cast<AAIController>(Grunt1->GetController())->GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetActorLocation());
        Cast<AAIController>(Grunt1->GetController())->GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), OtherActor->GetActorLocation()); */
        
        Location = FVector(5624.910156f, -5718.683105f, 525.696655f);
        SpawnInfo.Name = TEXT("QRF Grunt2");
        Grunt1 =GetWorld()->SpawnActor<AShooterCharacter>(ClassToSpawn, Location, Rotation, SpawnInfo);
        Controller1 = Cast<AShooterAIController>(Grunt1->GetController());
        if (!Controller1) return;
        Controller1->BeginInvestigation(GetActorLocation());

        Location = FVector(4874.72168f, -5646.288086f, 686.35199f);
        SpawnInfo.Name = TEXT("QRF Grunt3");
        Grunt1 =GetWorld()->SpawnActor<AShooterCharacter>(ClassToSpawn, Location, Rotation, SpawnInfo);
        Controller1 = Cast<AShooterAIController>(Grunt1->GetController());
        if (!Controller1) return;
        Controller1->BeginInvestigation(GetActorLocation());

        Activated = true;
    }
}

