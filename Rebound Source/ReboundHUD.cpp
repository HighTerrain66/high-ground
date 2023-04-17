// Fill out your copyright notice in the Description page of Project Settings.


#include "ReboundHUD.h"
#include "ReboundHUDWidget.h"
#include "ThiefCharacter.h"

void AReboundHUD::BeginPlay()
{
    Super::BeginPlay();

    
    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UReboundHUDWidget>(GetWorld(), HUDWidgetClass);

        /** Make sure widget was created */
        if (HUDWidget)
        {
            UE_LOG(LogTemp, Display, TEXT("INITHUD"));
            /** Add it to the viewport */
            HUDWidget->AddToViewport();
            AThiefCharacter* ThiefOwner = Cast<AThiefCharacter>(GetOwningPawn());
            ThiefOwner->OnHint.BindUObject(HUDWidget, &UReboundHUDWidget::OnHintReceived);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Widget"));
    }
}
