// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Fps_BlueprintFunctionLibrary.generated.h"

class AProjectile;

UCLASS()
class SHOOTER_API UFps_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayUtils")
	static AActor* SpawnActorDeferred(
		const UObject* WorldContextObject,
		TSubclassOf<AProjectile> ProjectileClass,
		const FVector SpawnLocation,
		const FRotator SpawnRotation,
		AActor* SpawnActor);

	// Fps_BlueprintFunctionLibrary.h - Fix the FinishSpawn signature:
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayUtils")
	static void FinishSpawn(
		AProjectile* Projectile,
		const FVector& SpawnLocation,  // Changed from FVector to const FVector&
		const FRotator& SpawnRotation, // Changed from FRotator to const FRotator&
		const FVector& LaunchDir,      // Changed from FVector to const FVector&
		float GravityScale = 1.f);
};