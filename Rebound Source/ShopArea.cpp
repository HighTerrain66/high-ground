// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopArea.h"
#include "ThiefCharacter.h"
#include "UpgradeHandleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Internationalization/Text.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE “ShopItem” 

AShopArea::AShopArea()
{
    PrimaryActorTick.bCanEverTick = false;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    BaseMesh->SetupAttachment(RootComponent);

    SelectionText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TitlePrice"));
    SelectionText->SetupAttachment(BaseMesh);

    DescriptionText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Description"));
    DescriptionText->SetupAttachment(BaseMesh);
}

void AShopArea::BeginPlay()
{
    Super::BeginPlay();

    AThiefCharacter* Player = Cast<AThiefCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    UpgradeHandle = Player->GetUpgradeComp();

    UpgradeHandle->SetupComponent(this);

    //Ensure prices and mesh display is correct
    ItemChanged();
}

void AShopArea::GoRight(bool bPlaySound)
{
    if (SelectedItem == ESelectedItem::Dash3)// the last upgrade
    {
        SelectedItem = ESelectedItem::Walljump;
        //return;
    }
    else
    {
        int index = SelectedItem;
    index++;

    SelectedItem = static_cast<ESelectedItem>(index);
    }

    

    UE_LOG(LogTemp, Display, TEXT("Going right"));
    //SelectedItem++;
    ItemChanged();

    if (MoveSound && bPlaySound) UGameplayStatics::SpawnSoundAtLocation(this, MoveSound, GetActorLocation());
}

void AShopArea::GoLeft()
{
    if (SelectedItem == ESelectedItem::Walljump)// the first upgrade
    {
        SelectedItem = ESelectedItem::Dash3;
        //return;
    }
    else
    {
        int index = SelectedItem;
        index--;
        SelectedItem = static_cast<ESelectedItem>(index);
    }
    
    UE_LOG(LogTemp, Display, TEXT("Going left"));
    //SelectedItem++;
    ItemChanged();

    if (MoveSound) UGameplayStatics::SpawnSoundAtLocation(this, MoveSound, GetActorLocation());
}


void AShopArea::ItemChanged()
{
    if (SelectedItem >= ShopInventory.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Selected item out of shop inventory range"));
        return;
    }

    FShopItem NewItem = ShopInventory[SelectedItem];
    // Display mesh of new item
    BaseMesh->SetStaticMesh(NewItem.DisplayMesh);
    // Get name and price data for shop item
    FText ItemName = NewItem.ItemName;
    FText ItemPrice = FText::AsNumber(NewItem.Price);

    // Change text displays
    FFormatNamedArguments TextArgs;

    TextArgs.Add(TEXT("Name"), ItemName);
    if (NewItem.bItemSold)
    {
        UE_LOG(LogTemp, Display, TEXT("Item has been sold"));
        TextArgs.Add(TEXT("Price"), NSLOCTEXT("ShopItem", "Sold", "Sold"));
        SelectionText->SetTextRenderColor(FColor::Yellow);
        DescriptionText->SetTextRenderColor(FColor::Yellow);
        BaseMesh->SetMaterial(0, Bought);
    }
    else
    {
        TextArgs.Add(TEXT("Price"), ItemPrice);
        SelectionText->SetTextRenderColor(FColor::White);
        DescriptionText->SetTextRenderColor(FColor::White);
        BaseMesh->SetMaterial(0, NotBought);
    }
    
    SelectionText->SetText(FText::Format(NSLOCTEXT("ShopItem", "ShopItem","{Name} - {Price}"), TextArgs));
    FString FirstPartDescription = NewItem.Description;
    // Put a triple line break between the ability description and command
    DescriptionText->SetText(FirstPartDescription.Append(FString("<br><br><br>").Append(NewItem.Command)));
}


void AShopArea::MenuAccept(bool bPlaySound)
{
    FShopItem BoughtItem = ShopInventory[SelectedItem];

    if (BoughtItem.bItemSold == true)
    {
        if (FailedSound && bPlaySound) UGameplayStatics::SpawnSoundAtLocation(this, FailedSound, GetActorLocation());
        return;
    }

    if (UpgradeHandle->GetOrbs() < BoughtItem.Price)
    {
        SelectionText->SetText(NSLOCTEXT("ShopItem", "ShopItem", "INSUFFICIENT ORBS"));
        if (SuccessSound && bPlaySound) UGameplayStatics::SpawnSoundAtLocation(this, FailedSound, GetActorLocation());
        return;
    }

    // Buy selected item
    switch (SelectedItem)
    {
        case Walljump:
            UpgradeHandle->walljumpLevel = 1;
            UE_LOG(LogTemp, Display, TEXT("WJ bought %i"), UpgradeHandle->walljumpLevel);
            break;
        case Mantle1:
            if (UpgradeHandle->mantleLevel < 1)
            UpgradeHandle->mantleLevel = 1;
            UE_LOG(LogTemp, Display, TEXT("Mantle 1 bought"));
            break;
        case Slide1:
            UpgradeHandle->slideLevel = 1;
            UE_LOG(LogTemp, Display, TEXT("Slide1 bought"));
            break;
        case Mantle2:
            UpgradeHandle->mantleLevel = 2;
            UE_LOG(LogTemp, Display, TEXT("Mantle 2 bought"));
            break;
        case Haste:
            UpgradeHandle->hasteLevel = 1;
            UE_LOG(LogTemp, Display, TEXT("Haste 1 bought"));
            break;
        case Leap1:
            UE_LOG(LogTemp, Display, TEXT("Leap 1 bought"));
            UpgradeHandle->leapLevel = 1;
            break;
        case Slide2:
            UpgradeHandle->slideLevel = 2;
            UE_LOG(LogTemp, Display, TEXT("Slide 2 bought"));
            break;
        case Leap2:
            UE_LOG(LogTemp, Display, TEXT("Leap 2 bought"));
            UpgradeHandle->leapLevel = 2;
            Cast<AThiefCharacter>(UpgradeHandle->GetOwner())->JumpMaxCount = 2;
            break;
        case Dash1:
            if (UpgradeHandle->dashLevel < 1)
            UpgradeHandle->dashLevel = 1;
            break;
        case Plunge:
            UpgradeHandle->plungeLevel = 1;
            break;
        case Dash2:
        if (UpgradeHandle->dashLevel < 2)
            UpgradeHandle->dashLevel = 2;
            break;
        case Dash3:
            UpgradeHandle->dashLevel = 3;
            break;
    }

    if (SuccessSound && bPlaySound) UGameplayStatics::SpawnSoundAtLocation(this, SuccessSound, GetActorLocation());


    ShopInventory[SelectedItem].bItemSold = true;
    UpgradeHandle->AddOrbs(-BoughtItem.Price);


    ItemChanged();

}

void AShopArea::BuyAll()
{
    UpgradeHandle->AddOrbs(10000);
    for (FShopItem item : ShopInventory)
    {
        GoRight();
        MenuAccept();
    }

}


#undef LOCTEXT_NAMESPACE