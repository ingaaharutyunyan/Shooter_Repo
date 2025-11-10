// Fill out your copyright notice in the Description page of Project Settings.


#include "Fps_BlueprintFunctionLibrary.h"
#include "Shooter/Weapons/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// method that sets up a projectile of any type, without returning reference or initializing them in real world.
AProjectile* UFps_BlueprintFunctionLibrary::SpawnActorDeferred( 
	const UObject* WorldContextObject,
	TSubclassOf<AProjectile> ProjectileClass,
	const FVector SpawnLocation,
	const FRotator SpawnRotation,
	AActor* SpawnActor)
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->SpawnActorDeferred<AProjectile>(
		ProjectileClass,
		SpawnTransform,
		SpawnActor,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	}
	return nullptr;
}

// method that initializes projectile and determines its speed, gravity scale and direction
// Fps_BlueprintFunctionLibrary.cpp - Fix the FinishSpawn implementation:
void UFps_BlueprintFunctionLibrary::FinishSpawn(
	AProjectile* Projectile,
	const FVector& SpawnLocation,
	const FRotator& SpawnRotation,
	const FVector& LaunchDir,
	float GravityScale)
{
	if (!IsValid(Projectile)) return;

	if (UProjectileMovementComponent* PMC = Projectile->FindComponentByClass<UProjectileMovementComponent>())
	{
		PMC->ProjectileGravityScale = GravityScale;
		PMC->Velocity = LaunchDir.GetSafeNormal() * PMC->InitialSpeed;
		PMC->bRotationFollowsVelocity = true;
	}

	FTransform Xf(SpawnRotation, SpawnLocation);
	Projectile->FinishSpawning(Xf);
}