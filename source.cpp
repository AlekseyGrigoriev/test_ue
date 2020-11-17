// Copyright Epic Games, Inc. All Rights Reserved.
#include "BaseHUD.h"

#include "Blueprint/UserWidget.h"

#include "Screens/BattleScreen.h"


void ABaseHUD::Init(APawn* OwningPawn)
{
	CreateScreenWidgets();
	UBattleScreen* BattleScreen = GetScreen<UBattleScreen>();
	if (IsValid(BattleScreen))
	{
	    BattleScreen->InitByPawn(OwningPawn);
	}
}

void ABaseHUD::ShowHUD()
{
    Super::ShowHUD();

    SetBattleScreenVisibility(bShowHUD);
}

void ABaseHUD::HideHUD()
{
    SetBattleScreenVisibility(false);
}

void ABaseHUD::ShowBattleScreen()
{
    SetBattleScreenVisibility(true);
}

void ABaseHUD::SetBattleScreenVisibility(const bool bShouldBeVisible)
{
    UBattleScreen* BattleScreen = GetScreen<UBattleScreen>();
    if (IsValid(BattleScreen))
    {
        BattleScreen->SetVisibility(bShouldBeVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
    }
    
    GetOwningPlayerController()->SetVirtualJoystickVisibility(bShouldBeVisible);
}

void origin_rel_2()
{
	
}

void ABaseHUD::AddWidgetToScreen(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!ScreensMap.Contains(WidgetClass))
	{
		UUserWidget* ScreenWidget = CreateWidget(GetOwningPlayerController(), WidgetClass);
		if (IsValid(ScreenWidget))
		{
			ScreenWidget->AddToViewport();
			ScreensMap.Add(WidgetClass, ScreenWidget);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Couldn't create widget for class %s"), *WidgetClass->GetName());
		}
	}
}

void ABaseHUD::CreateScreenWidgets()
{
	for (TSubclassOf<UUserWidget> WidgetClass : InitialScreensClasses)
	{
		if (!IsValid(WidgetClass))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid WidgetClass in InitialScreensClasses"));
			continue;
		}
		AddWidgetToScreen(WidgetClass);
	}
	
#if TS_BUILD_CHEATS
	AddWidgetToScreen(CheatPanelClass);
#endif
}
