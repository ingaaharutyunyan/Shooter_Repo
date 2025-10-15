#include "Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" // ✅ Add this line

AGrenade::AGrenade()
{
	// More powerful, slower projectile
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = 800.0f;
		ProjectileMovementComponent->MaxSpeed = 800.0f;
		ProjectileMovementComponent->ProjectileGravityScale = 1.5f; // heavier arc
	}
}

// Add to Grenade.cpp:
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
    
	// Start explosion timer
	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimer,
		this,
		&AGrenade::Explode,
		ExplosionDelay,
		false
	);
}

void AGrenade::DealDamage()
{
	// Optional immediate damage behavior
	Explode();
}

void AGrenade::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("AGrenade: BOOM! Applying radial damage."));

	UGameplayStatics::ApplyRadialDamage(
		this,
		ExplosionDamage,
		GetActorLocation(),
		ExplosionRadius,
		nullptr,
		{},           // ignored actors
		this,
		GetInstigatorController(),
		true
	);

	Destroy();
}
