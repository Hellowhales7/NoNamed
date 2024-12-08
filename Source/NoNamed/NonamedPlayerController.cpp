// Fill out your copyright notice in the Description page of Project Settings.


#include "NonamedPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
void ANonamedPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromViewport();
	if (bIsWinner) {
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else {
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartTime);
}

void ANonamedPlayerController::BeginPlay()
{
	Super::BeginPlay();
	HUD = CreateWidget(this, HeadUpDisplay);
	if(HUD != nullptr)
		HUD->AddToViewport();
}
