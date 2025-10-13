// ShooterGameMode.cpp
#include "ShooterGameMode.h"
#include "PlayerManager.h"

AShooterGameMode::AShooterGameMode()
{
	// Set the default pawn class to our PlayerManager
	DefaultPawnClass = APlayerManager::StaticClass();
}