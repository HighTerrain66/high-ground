// Fill out your copyright notice in the Description page of Project Settings.


#include "ReboundHUDWidget.h"
#include "ThiefCharacter.h"

bool UReboundHUDWidget::Initialize()
{

    bool bSuccess = Super::Initialize();

    AThiefCharacter* ThiefOwner = Cast<AThiefCharacter>(GetOwningPlayerPawn());

    //ThiefOwner->OnHint.BindUObject(this, &UReboundHUDWidget::OnHintReceived);

    return bSuccess;
}

/* void UReboundHUDWidget::OnHintReceived(FString Hint)
{
    UE_LOG(LogTemp, Warning, TEXT("YES LET@S GO %s"), *Hint)
}
void UReboundHUDWidget::OnHintReceived(FString Hint)
{
    
} */
