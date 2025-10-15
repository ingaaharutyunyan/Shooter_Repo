// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Blueprint.h"

AShooterGameMode::AShooterGameMode()
{
	// Set default pawn class to your Blueprint character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/MainCharacter_BP"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, 
			TEXT("Could not find blueprint of player"));
	}
}