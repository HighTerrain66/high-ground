// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ShopArea.generated.h"

/**
 * 
 */

UENUM()
enum ESelectedItem
{
  Walljump     UMETA(DisplayName = "Walljump"),
  Mantle1      UMETA(DisplayName = "Mantle1"),
  Slide1      UMETA(DisplayName = "Slide1"),
  Mantle2      UMETA(DisplayName = "Mantle2"),
  Haste         UMETA(DisplayName = "Haste"),
  Leap1          UMETA(DisplayName = "Leap1"),
  Slide2      UMETA(DisplayName = "Slide2"),
  Leap2,
  Dash1,
  Plunge,
  Dash2,
  Dash3,
};

USTRUCT(BlueprintType)
struct FShopItem
{
  GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;
  UPROPERTY(EditAnywhere)
  bool bItemSold = false;
  UPROPERTY(EditAnywhere)
  int32 Price = 20;
  UPROPERTY(EditAnywhere)
  FString Description = "Useful upgrade";
  UPROPERTY(EditAnywhere)
  UStaticMesh* DisplayMesh;
  UPROPERTY(EditAnywhere)
  FString Command = "Press button";
};

UCLASS()
class REBOUND_API AShopArea : public ATriggerBox
{
	GENERATED_BODY()
	

	public:

  AShopArea();
  void GoRight(bool bPlaySound = true);
  void GoLeft();

  void MenuAccept(bool bPlaySound = true);
  UPROPERTY(EditAnywhere, meta = (TitleProperty = "ItemName"))
  TArray<FShopItem> ShopInventory;
  
	private:
	UPROPERTY(EditAnywhere)
	TArray<class UStaticMesh*> DisplayMeshes;

  UPROPERTY(EditAnywhere)
  class UMaterialInterface* NotBought;
  UPROPERTY(EditAnywhere)
  class UMaterialInterface* Bought;
  

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  void ItemChanged();

  UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BaseMesh;

  UPROPERTY()
  class UUpgradeHandleComponent* UpgradeHandle;

  UPROPERTY(VisibleAnywhere)
  class UTextRenderComponent* SelectionText;

  UPROPERTY(VisibleAnywhere)
  class UTextRenderComponent* DescriptionText;

  private:

  ESelectedItem SelectedItem = ESelectedItem::Walljump;

  void BuyAll();  

  void BuyWalljump();
  void BuyMantle1();
  void BuySlide1();
  void BuyMantle2();
  void BuyHaste();
  void BuySlide2();

  UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Move Sound")
	class USoundBase* MoveSound;
	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Success Sound")
	class USoundBase* SuccessSound;
	UPROPERTY(EditDefaultsOnly, Category = "SFX", DisplayName = "Failed Sound")
	class USoundBase* FailedSound;
};
