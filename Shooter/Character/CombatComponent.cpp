#include "C:\Users\inga.harutyunyan\Documents\Unreal Projects\Shooter\Intermediate\Build\Win64\x64\ShooterEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.h"
#include "../Weapons/Grenade.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CombatComponent.h"



void UCombatComponent::SpawnProjectile()
{
	AActor* Owner = GetOwner();
	AActor* ParentActor = Owner ? Owner->GetAttachParentActor() : nullptr;
	AActor* ReferenceActor = ParentActor ? ParentActor : Owner; // fallback to owner

	if (GetWorld() && ReferenceActor)
	{
		FVector SpawnLocation = ReferenceActor->GetActorLocation() + ReferenceActor->GetActorForwardVector() * 100.0f;
		FRotator SpawnRotation = ReferenceActor->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = ReferenceActor; // sets owner of projectile to our player
		SpawnParams.Instigator = ReferenceActor->GetInstigator(); // sets the gameobject that started the action to our player
		
		CurrentProjectile = UFps_BlueprintFunctionLibrary::SpawnActorDeferred(GetWorld(), AProjectile::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void UCombatComponent::OnAimPressed()
{
	SpawnProjectile();
	Aim();
}

// CombatComponent.cpp - Fix the OnThrowPressed method:
void UCombatComponent::OnThrowPressed()
{
	if (CurrentProjectile && GetWorld())
	{
		// Get camera direction from owner
		AActor* Owner = GetOwner();
		if (Owner)
		{
			FVector CameraDirection = Owner->GetActorForwardVector(); // Simple fallback
			// For more accurate camera direction, you'd need to access the camera component
            
			UFps_BlueprintFunctionLibrary::FinishSpawn(
				CurrentProjectile, 
				CurrentProjectile->GetActorLocation(), 
				CurrentProjectile->GetActorRotation(), 
				CameraDirection, 
				1.0f
			);
			Throw();
		}
	}
}

void UCombatComponent::Aim()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Projectile: Aiming"));

	FVector Start = CurrentProjectile->GetActorLocation();
	FVector End = Start + CurrentProjectile->GetActorForwardVector() * 1000.0f;

	FHitResult HitResult;
	const TArray<AActor*> ActorsToIgnore = { this };
	float DrawTime = 5.0f;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	// UGameplayStatics::PredictProjectilePath
	bool bHit = UKismetSystemLibrary::LineTraceSingle( // change to linetracesinglebychannel
		GetWorld(),
		Start,
		End,
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, 
		HitResult,
		true,
		FColor::Red,
		FColor::Green,
		DrawTime
	);

	if (bHit && HitResult.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
	}
}


void UCombatComponent::Throw()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Projectile: Thrown"));
}