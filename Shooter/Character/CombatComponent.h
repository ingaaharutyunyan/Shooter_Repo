#pragma once

#include "CoreMinimal.h"
#include "../Utils/Fps_BlueprintFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	void OnAimPressed();
	void OnThrowPressed();

	// Base projectile behavior
	UFUNCTION(BlueprintCallable, Category = "Aim and Throw")
	virtual void Aim();

	UFUNCTION(BlueprintCallable, Category = "Aim and Throw")
	virtual void Throw();

private:
	UPROPERTY()
	class AProjectile* CurrentProjectile;

	void SpawnProjectile();
};
