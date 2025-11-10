#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Grenade.generated.h"

UCLASS()
class SHOOTER_API AGrenade : public AProjectile
{
	GENERATED_BODY()

public:
	AGrenade();
	virtual void BeginPlay() override;
	virtual void DealDamage() override;
	

protected:
	// Explosion settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float ExplosionDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float ExplosionRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float ExplosionDamage = 100.0f;

private:
	FTimerHandle ExplosionTimer;
	void Explode();
};



