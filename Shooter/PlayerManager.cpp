// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Weapons/Projectile.h"

// Sets default values
APlayerManager::APlayerManager()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Add your mapping context
				Subsystem->AddMappingContext(InputMappingPlayer, 0);
                
				// Bind using EnhancedInputComponent
				EnhancedInputComponent->BindAction(AimProjectile, ETriggerEvent::Started, this, &APlayerManager::OnAimPressed);
				EnhancedInputComponent->BindAction(ThrowProjectile, ETriggerEvent::Completed, this, &APlayerManager::OnThrowPressed);
			}
		}
	}
}

void APlayerManager::SpawnProjectile()
{
	if (GetWorld())
	{
		// Get player's location and rotation
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
		FRotator SpawnRotation = GetActorRotation();
        
		// Spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
        
		// Spawn the projectile
		CurrentProjectile = GetWorld()->SpawnActor<AProjectile>(
			AProjectile::StaticClass(), 
			SpawnLocation, 
			SpawnRotation, 
			SpawnParams
		);
        
		if (CurrentProjectile)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Projectile Spawned!"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn projectile!"));
		}
	}
}

void APlayerManager::OnAimPressed()
{
	if (CurrentProjectile == nullptr) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnAimPressed CALLED - No projectile, spawning one"));
		SpawnProjectile(); // Spawn when aiming if none exists
	}
	else 
	{
		CurrentProjectile->Aim();
	}
}

void APlayerManager::OnThrowPressed()
{GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("OnThrowPressed Called!"));
	CurrentProjectile->Throw();
}
