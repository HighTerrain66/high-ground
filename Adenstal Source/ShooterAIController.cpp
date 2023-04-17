// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacterAIComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "DrawDebugHelpers.h"
#include "Perception/AIPerceptionComponent.h" 

FString GetEnumText(AIState Role)
{
	switch (Role)
	{
	case LOITER:
		return "LOITER";
	case PATROL:
		return "PATROL";
	case CHASE:
		return "CHASE";
	case ATTACK:
		return "ATTACK";
    case INVESTIGATE:
		return "INVESTIGATE";
	default:
		return "ERROR";
	}
}

AShooterAIController::AShooterAIController() 
{
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
}

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    //PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AShooterAIController::PerceptionUpdate);
}

void AShooterAIController::OnPossess(APawn* InPawn) 
{
    Super::OnPossess(InPawn);
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (AIBehavior != nullptr)
    {
        APawn* ThisPawn = GetPawn();
        /* RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        if (ThisPawn != nullptr && GetBlackboardComponent()->GetValueAsVector(TEXT("StartLocation")).IsZero())
        {
            GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), ThisPawn->GetActorLocation());
        } */

        //ERROR HERE FROM CONSOLETRIGGER1
        if (IsValid(ThisPawn))
        {
            StartLocation = ThisPawn->GetActorLocation();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AI Controller without Pawn"));
            return;
        }
        

        UShooterCharacterAIComponent* AIComponent = Cast<UShooterCharacterAIComponent>(ThisPawn->FindComponentByClass(UShooterCharacterAIComponent::StaticClass()));
        if (AIComponent)
        {
            AssignAIValues(AIComponent);

            GetWorld()->GetTimerManager().SetTimer(TimerHandle_AITick, this, &AShooterAIController::AITick, 0.3f, true, 0.3f);

            
            CharacterMovementComponent = Cast<ACharacter>(GetPawn())->GetCharacterMovement();
            if (!CurrentState)
            {
                if (HighestWaypointIndex > 0)
                {
                    CurrentState = AIState::PATROL;
                }
                else
                {
                    CurrentState = AIState::LOITER;
                }

                CharacterMovementComponent->MaxWalkSpeed = PatrolMoveSpeed;
            }
            else
            {
                CharacterMovementComponent->MaxWalkSpeed = CombatMoveSpeed;
            }
            OldState = CurrentState;

            AIComponent->AIState = GetEnumText(CurrentState);
        }
    }
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    
    
}



void AShooterAIController::AITick()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn == nullptr) return;

    //UE_LOG(LogTemp, Display, TEXT("AITICK %s"), *GetEnumText(CurrentState));
    if (CurrentState != OldState)
    {
        //UE_LOG(LogTemp, Warning, TEXT("AI state change from %s to %s"), *GetEnumText(OldState), *GetEnumText(CurrentState))
        OldState = CurrentState;
        
        UShooterCharacterAIComponent* AIComponent = Cast<UShooterCharacterAIComponent>(GetPawn()->FindComponentByClass(UShooterCharacterAIComponent::StaticClass()));
        AIComponent->AIState = GetEnumText(CurrentState);
    }

    /* f (CurrentState == AIState::LOITER)
    {
        Loiter();
    }
    else if (CurrentState == AIState::PATROL) Patrol();
    else if (CurrentState == AIState::CHASE)
    {
        UE_LOG(LogTemp, Display, TEXT("AI TICK CHASING"));
        Chase();
    } 
    else if (CurrentState == AIState::ATTACK)
    {
        UE_LOG(LogTemp, Display, TEXT("AI TICK CHASING"));
        Attack();
    } 
    else if (CurrentState == AIState::INVESTIGATE) Investigate(); */
    switch (CurrentState) {
	case 0:
		Loiter();
        break;
	case 1:
		Patrol();
        break;
	case 2:
        //UE_LOG(LogTemp, Display, TEXT("Chasing"));
		Chase();
        break;
	case 3:
    //UE_LOG(LogTemp, Display, TEXT("Attacking"));
		Attack();
        break;
    case 4:
        Investigate();
        break;
	default:
        UE_LOG(LogTemp, Error, TEXT("SWITCH NOT WORKING %s"), *GetEnumText(CurrentState));
		Loiter();
        break;
	}

    
}

void AShooterAIController::Loiter() 
{
    EPathFollowingRequestResult::Type Result = MoveToLocation(StartLocation, 40, false, true, true, false);
    Perception();

    /* if (Result == EPathFollowingRequestResult::Type::Failed)
    {
        UE_LOG(LogTemp, Error, TEXT("LOITER FAILED"));
    }
    else if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
    {
        UE_LOG(LogTemp, Display, TEXT("LOITER SUCCESSFUL"));
        
    } */
}

void AShooterAIController::Patrol() 
{
    EPathFollowingRequestResult::Type Result = MoveToLocation(PatrolLocation, 40, false, true, true, false);
    //UE_LOG(LogTemp, Display, TEXT("TimeRemaining: %f"), GetWorldTimerManager().GetTimerRemaining(TimerHandle_NextPatrolPoint))
    if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
    {
        //UE_LOG(LogTemp, Display, TEXT("NEXT PATROL POINT YES"));
        
        if (GetWorldTimerManager().GetTimerRemaining(TimerHandle_NextPatrolPoint) == -1.f)
        {
            //UE_LOG(LogTemp, Display, TEXT("NEXT PATROL POINT YES, %f"), WaitTime);
            GetWorld()->GetTimerManager().SetTimer(TimerHandle_NextPatrolPoint, this, &AShooterAIController::NextPatrolPoint, 4.f, false, WaitTime);
        }
        
    }
    else if (Result == EPathFollowingRequestResult::Type::Failed)
    {
        UE_LOG(LogTemp, Error, TEXT("PATROL FAILED"));
    }
    
    Perception();
}

void AShooterAIController::Chase() 
{
    UE_LOG(LogTemp, Display, TEXT("CHASING tick"));
    bool Proceed = IsTargetStillInSight();
    if (!Proceed) return;

    EPathFollowingRequestResult::Type Result;
    Result = MoveToActor(Target, 1200, true, true, true);
    if (Result != EPathFollowingRequestResult::Type::RequestSuccessful)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_AIAttack, this, &AShooterAIController::AIAttack, 0.5f, true, 1.f);
        CurrentState = AIState::ATTACK;
    }

}

void AShooterAIController::Attack() 
{
    UE_LOG(LogTemp, Display, TEXT("ATTACKING"));
    bool Proceed = IsTargetStillInSight();
    if (!Proceed) return;

    if ((Target->GetActorLocation() - GetPawn()->GetActorLocation()).SizeSquared() > 2000000)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_AIAttack, this, &AShooterAIController::AIAttack, 0.8f, true, 1.f);
        CurrentState = AIState::CHASE;
    }
}

void AShooterAIController::Investigate() 
{
    EPathFollowingRequestResult::Type Result = MoveToLocation(InvestigationLocation, 40, true, true, true);
    if (Result == EPathFollowingRequestResult::Type::AlreadyAtGoal && !TimerHandle_BreakOff.IsValid())
    {
        CurrentInvestigationPriority = 0;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_BreakOff, this, &AShooterAIController::BreakOff, 0.8f, false, 2.f);
    }

    Perception();
}

// Return to noncombat state
void AShooterAIController::BreakOff() 
{
    //UE_LOG(LogTemp, Display, TEXT("Lost Target!"));
    GetWorldTimerManager().ClearTimer(TimerHandle_AIAttack);
    ClearFocus(EAIFocusPriority::Gameplay);
    CharacterMovementComponent->MaxWalkSpeed = PatrolMoveSpeed;
    if (HighestWaypointIndex > 0)
    {
        CurrentState = AIState::PATROL;
    }
    else
    {
        CurrentState = AIState::LOITER;
    }
}

void AShooterAIController::BeginEngagement() 
{
    if (!IsValid(GetPawn())) return;
    UE_LOG(LogTemp, Display, TEXT("ENGAGING"));
    GetWorldTimerManager().ClearTimer(TimerHandle_BreakOff);
    //GetWorldTimerManager().ClearTimer(TimerHandle_);
    // just in case target is nullptr for whatever reason
    if (Target == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Target is nullptr"));
        BreakOff();
        return;
    }

    if (CharacterMovementComponent != nullptr) CharacterMovementComponent->MaxWalkSpeed = CombatMoveSpeed;

    SetFocus(Target, EAIFocusPriority::Gameplay);

    if ((Target->GetActorLocation() - GetPawn()->GetActorLocation()).SizeSquared() < 490000 || bIsStatic)
    {
        UE_LOG(LogTemp, Display, TEXT("ATTACKING"));
        CurrentState = AIState::ATTACK;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_AIAttack, this, &AShooterAIController::AIAttack, 0.4f, true, 0.7f);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("CHASING"));
        CurrentState = AIState::CHASE;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_AIAttack, this, &AShooterAIController::AIAttack, 0.7f, true, 0.8f);
    }
}

void AShooterAIController::AIAttack() 
{
    AShooterCharacter* Shooter = Cast<AShooterCharacter>(GetPawn());
    if (Shooter == nullptr)
    {
        return;
    }
    if (Target == nullptr || (Cast<AShooterCharacter>(Target) && Cast<AShooterCharacter>(Target)->IsDead()))
    {
        Target = nullptr;
        BreakOff();
    }

    Shooter->EnterCombat();
    Shooter->PlayAnimMontage(FireMontage, 1.2f);
    Shooter->SetFiringSpread(0.2f);
    Shooter->TriggerPull();

    if (!bHasEngagedYet)
    {
        bHasEngagedYet = true;
        FirstAlert.Broadcast();
    }
}

void AShooterAIController::NextPatrolPoint() 
{
    GetWorldTimerManager().ClearTimer(TimerHandle_NextPatrolPoint);
    if (WaypointIndex >= HighestWaypointIndex) WaypointIndex = 0;
    else WaypointIndex++;

    UShooterCharacterAIComponent* AIComponent = Cast<UShooterCharacterAIComponent>(GetPawn()->FindComponentByClass(UShooterCharacterAIComponent::StaticClass()));
    if (AIComponent)
    {
        PatrolLocation = AIComponent->GetWaypoints()[WaypointIndex];
        UE_LOG(LogTemp, Display, TEXT("NEXT PATROL POINT"));
    }
}

bool AShooterAIController::IsTargetStillInSight()
{
    UE_LOG(LogTemp, Display, TEXT("TRACKING TARGET"));
    // If this AI can see the player
    if (Target == nullptr) {return false;}
    if (LineOfSightTo(Target) && GetPawn()->GetTransform().InverseTransformPosition(Target->GetActorLocation()).Y > -400)
    {
        return true;
        /* if (Target != nullptr)
        {
            UE_LOG(LogTemp, Display, TEXT("FEINDKONTACT"));
            Cast<AShooterCharacter>(GetPawn())->DeclareContact();
        }
        
        GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), PlayerPawn);
        Target = PlayerPawn; */
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Lost Target!"));
        GetWorldTimerManager().ClearTimer(TimerHandle_AIAttack);
        FVector TestLocation = Target->GetActorLocation();
        // looks for where target disappeared if they were 30 m away or less
        if (true)//(TestLocation - GetPawn()->GetActorLocation()).SizeSquared() < 180000
        {
            UE_LOG(LogTemp, Display, TEXT("Investigating"));
            CurrentInvestigationPriority = 3;
            BeginInvestigation(TestLocation);
        }
        else
        {
            
            BreakOff();
        }
        
        Target = nullptr;

        return false;
    }
}

bool AShooterAIController::CanSeeActor_NonCombat(AActor* Actor) const
{
    if (bIsBlind|| !LineOfSightTo(Actor)) return false;
    FVector RPosition = GetPawn()->GetTransform().InverseTransformPosition(Actor->GetActorLocation());
    if (!(RPosition.X > -400)) return false;
    if (!(RPosition.Z > -800)) return false;
    if (!(RPosition.Z < 800)) return false;

    return true;
}

void AShooterAIController::Perception() 
{
    // looks for only one playerpawn for now
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (PlayerPawn == nullptr)
    {
        return;
    }

    if (!PlayerPawn->GetActorLocation().IsZero())
    {

    }
    // If this AI can see the player
    if (CanSeeActor_NonCombat(PlayerPawn))
    {   
        if (Target == nullptr)
        {
            //UE_LOG(LogTemp, Display, TEXT("FEINDKONTACT"));
            Cast<AShooterCharacter>(GetPawn())->DeclareContact();
        }
        
        Target = PlayerPawn;
        BeginEngagement();
    }
}

void AShooterAIController::AssignAIValues(class UShooterCharacterAIComponent* AIComponent) 
{
    WaitTime = AIComponent->GetWaitTime();
    CurrentWaitTime = WaitTime;
    HighestWaypointIndex = AIComponent->GetWaypoints().Num() - 1;
    if (HighestWaypointIndex < 0) {HighestWaypointIndex = 0;}
    else {PatrolLocation = AIComponent->GetWaypoints()[0];}

    CombatMoveSpeed = AIComponent->CombatMoveSpeed;
    PatrolMoveSpeed = AIComponent->PatrolMoveSpeed;

    bIsStatic = AIComponent->bIsStatic;
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}

void AShooterAIController::OnTakeDamage(AActor* Cause) 
{
    Target = Cause;
    // this will also effectively stagger the enmy after being hit, resetting attack timers
    BeginEngagement();
}

void AShooterAIController::BeginInvestigation(FVector Position) 
{
    InvestigationLocation = Position;
    CurrentState = AIState::INVESTIGATE;
}
/* 
void AShooterAIController::PerceptionUpdate(const TArray<AActor*>& UpdatedActors) 
{
    for (AActor* UpdatedActor : UpdatedActors)
    {
        
    }
}
 */
void AShooterAIController::RegisterNoise(FVector Location, uint8 Priority, AActor* SourceActor, FString Reason) 
{
    if (bIsStatic) return;

    //UE_LOG(LogTemp, Display, TEXT("PAWN DETECTED NOISE"));

    AShooterCharacter* NoisyShooter = Cast<AShooterCharacter>(SourceActor);
    if (NoisyShooter)
    {
        if (NoisyShooter->Team == Cast<AShooterCharacter>(GetPawn())->Team)
        {
            if (Reason == "Step") Priority -= 2;// stops ai from investigating footsteps of friendly ai
            else if (Reason == "Callout")
            {
                
            }
        }
    }

    if (Priority >= CurrentInvestigationPriority)
    {
        Cast<AShooterCharacter>(GetPawn())->DetectNoise();
        //DrawDebugSphere(GetWorld(), Location, 50, 20, FColor::Purple, false, 2, 0, 2);
        //DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), 50, 20, FColor::Yellow, false, 2, 0, 2);
        //UE_LOG(LogTemp, Display, TEXT("PAWN ENGAGING NOISE"));
        BeginInvestigation(Location);
        CurrentInvestigationPriority = Priority;
    }
}


