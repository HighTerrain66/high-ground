// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeHandleComponent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOrbCountChangeDelegate, int32, Change);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REBOUND_API UUpgradeHandleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpgradeHandleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float mantleSpeed1 = 250.f;

	UPROPERTY(EditAnywhere)
	float mantleSpeed2 = 400.f;

	
	UPROPERTY(EditAnywhere)
	TArray<float> hasteSpeeds;

	int32 Orbs = 30;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupComponent(class AShopArea* InObject);

	void ShopRight();
	void ShopLeft();

	void ShopAccept();

	void CheatBuyAll();

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 mantleLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 walljumpLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 slideLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 dashLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 plungeLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 hasteLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 leapLevel = 0;

	UPROPERTY(BlueprintAssignable)
	FOrbCountChangeDelegate OrbCountChanged;

	UFUNCTION(BlueprintPure)
	int32 GetOrbs() {return Orbs;}

	UFUNCTION()
	void AddOrbs(int32 Input); 


private:
	UPROPERTY()
	class AShopArea* ShopObject;

	
	
		
};
