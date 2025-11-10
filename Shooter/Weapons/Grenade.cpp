#include "Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGrenade::AGrenade()
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = 800.0f;
		ProjectileMovementComponent->MaxSpeed = 800.0f;
		ProjectileMovementComponent->ProjectileGravityScale = 1.5f;
	}
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
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
		{},           
		this,
		GetInstigatorController(),
		true
	);

	Destroy();
}
