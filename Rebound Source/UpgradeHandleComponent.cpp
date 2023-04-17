// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeHandleComponent.h"
#include "ShopArea.h"
#include "ThiefMovementComponent.h"
#include "ThiefCharacter.h"

// Sets default values for this component's properties
UUpgradeHandleComponent::UUpgradeHandleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	// ...
}


// Called when the game starts
void UUpgradeHandleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UThiefMovementComponent* MoveComp = Cast<UThiefMovementComponent>(Cast<AThiefCharacter>(GetOwner())->GetMovementComponent());

	MoveComp->SlideLevel = slideLevel;
}

void UUpgradeHandleComponent::SetupComponent(AShopArea* InObject)
{
	ShopObject = InObject;
}


// Called every frame
void UUpgradeHandleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UUpgradeHandleComponent::ShopRight()
{

	if (ShopObject== nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No shopobject"));

		return;
	}
	ShopObject->GoRight();
}

void UUpgradeHandleComponent::ShopLeft()
{

	if (ShopObject== nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No shopobject"));

		return;
	}
	ShopObject->GoLeft();
}

void UUpgradeHandleComponent::ShopAccept()
{
	UE_LOG(LogTemp, Display, TEXT("Shopaccept"));
	ShopObject->MenuAccept();

	UThiefMovementComponent* MoveComp = Cast<UThiefMovementComponent>(Cast<AThiefCharacter>(GetOwner())->GetMovementComponent());

	MoveComp->SlideLevel = slideLevel;
	MoveComp->DashLevel = dashLevel;
	MoveComp->WalljumpLevel = walljumpLevel;

	if (hasteSpeeds.Num() > 1)
	MoveComp->MaxWalkSpeed = hasteSpeeds[hasteLevel];

	if (leapLevel == 1)
	
		MoveComp->JumpBoost = 150.f;
	
	
}

void UUpgradeHandleComponent::CheatBuyAll()
{
	Orbs += 10000;
    for (FShopItem item : ShopObject->ShopInventory)
    {
        ShopObject->GoRight(false);
        ShopObject->MenuAccept(false);
		FString name = item.ItemName.ToString();
		UE_LOG(LogTemp, Display, TEXT("itum, %s"), *name);
		
    }
	ShopAccept();
}

void UUpgradeHandleComponent::AddOrbs(int32 Input)
{
	Orbs += Input;
	if (OrbCountChanged.IsBound())
	{
		OrbCountChanged.Broadcast(Input);
	}
}
