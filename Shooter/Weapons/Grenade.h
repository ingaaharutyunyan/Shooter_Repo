#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Grenade.generated.h"

UCLASS()
class SHOOTER_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AGrenade();
	
	UFUNCTION(BlueprintCallable, Category = "Grenade")
	virtual void Throw();

	UFUNCTION(BlueprintCallable, Category = "Grenade")
	virtual void Aim();

	UFUNCTION(BlueprintCallable, Category = "Grenade")
	virtual void DealDamage();
};