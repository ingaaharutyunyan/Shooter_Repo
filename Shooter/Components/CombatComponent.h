#pragma once

#include "CoreMinimal.h"
#include "../Utils/Fps_BlueprintFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Templates/SubclassOf.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	void OnAimPressed(TSubclassOf<AProjectile> ProjectileClass);
	void OnThrowPressed();

	UFUNCTION(BlueprintCallable, Category = "Aim and Throw")
	virtual void Aim();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AProjectile> GrenadeBlueprintClass;

private:
	UPROPERTY()
	class AProjectile* CurrentProjectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trajectory", meta = (AllowPrivateAccess = "true"))
	USplineComponent* TrajectorySpline;

	UFUNCTION()
	void SpawnProjectile(TSubclassOf<AProjectile> ProjectileClass);

	void PredictTrajectory(const FVector& StartLocation, const FVector& LaunchDirection);
	void ClearTrajectory();
	FPredictProjectilePathResult PredictResult;
	FVector CachedLaunchVelocity;
};