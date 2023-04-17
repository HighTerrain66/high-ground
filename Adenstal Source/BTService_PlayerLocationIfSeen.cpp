// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BTService.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Math/Vector.h"
#include "ShooterCharacter.h"
#include "ShooterCharacterAIComponent.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (PlayerPawn == nullptr)
    {
        return;
    }
    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return;
    }

    AShooterCharacter* ParentActor = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (ParentActor == nullptr) {return;}
    
    UShooterCharacterAIComponent* AIComponent = Cast<UShooterCharacterAIComponent>(ParentActor->FindComponentByClass(UShooterCharacterAIComponent::StaticClass()));
    if (AIComponent == nullptr) return;

    if (init) 
    {
        // Extracts parameters set in component attached to character
        AssignAIValues(AIComponent);
        
        // Got some crashes here before so have a long if condition
        if (HighestWaypointIndex > 0 && HighestWaypointIndex < AIComponent->GetWaypoints().Num())
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), AIComponent->GetWaypoints()[0]);
        }
    }
    // If this AI can see the player
    if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn) && OwnerComp.GetAIOwner()->GetPawn()->GetTransform().InverseTransformPosition(PlayerPawn->GetActorLocation()).Y > -400)
    {   
        //UE_LOG(LogTemp, Display, TEXT("NAME: %s"), *OwnerComp.GetOwner()->GetName());
        
        if (!OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")) && ParentActor)
        {
            //UE_LOG(LogTemp, Display, TEXT("FEINDKONTACT"));
            //Cast<AShooterCharacter>(OwnerComp.GetOwner())->DeclareContact();
        }
        
        //OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), PlayerPawn);
    }
    else
    {
        // Unset the Player blackboard value and take note of their last known location
        //UE_LOG(LogTemp, Warning, TEXT("Things are happening"));
        if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")))
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
            OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("Player"));
        }
        
        // Wait at patrol waypoint for period of waitime before moving to next waypoint
        if (HighestWaypointIndex <= 0) return;
        FVector Displacement = OwnerComp.GetOwner()->GetActorLocation() - OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("PatrolLocation"));
        if (Displacement.SizeSquared() < 1)
        {
            CurrentWaitTime -= DeltaSeconds;
            if (CurrentWaitTime < 0)
            {
                if (WaypointIndex >= HighestWaypointIndex) WaypointIndex = 0;
                else WaypointIndex++;
                OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolLocation"), AIComponent->GetWaypoints()[WaypointIndex]);
                
                CurrentWaitTime = WaitTime;
            }
        }
    }
}

void UBTService_PlayerLocationIfSeen::AssignAIValues(UShooterCharacterAIComponent* AIComponent) 
{
    WaitTime = AIComponent->GetWaitTime();
    CurrentWaitTime = WaitTime;
    HighestWaypointIndex = AIComponent->GetWaypoints().Num() - 1;
    if (HighestWaypointIndex < 0) {HighestWaypointIndex = 0;}
    init = false;
}
