#include "C:\Users\inga.harutyunyan\Documents\Unreal Projects\Shooter\Intermediate\Build\Win64\x64\ShooterEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.h"
#include "../Weapons/Grenade.h"
#include "CombatComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/SplineComponent.h" 

UCombatComponent::UCombatComponent()
{
    TrajectorySpline = CreateDefaultSubobject<USplineComponent>(TEXT("TrajectorySpline"));
}

void UCombatComponent::SpawnProjectile(TSubclassOf<AProjectile> ProjectileClass)
{
    AActor* Owner = GetOwner();
    if (!IsValid(GetWorld()) || !IsValid(Owner)) return;

    FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.0f + FVector(0, 0, 50.0f);
    FRotator SpawnRotation = Owner->GetActorRotation();

    if (IsValid(GrenadeBlueprintClass))
    {
        ProjectileClass = GrenadeBlueprintClass;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Using Blueprint Grenade Class"));
    }
    
    CurrentProjectile = UFps_BlueprintFunctionLibrary::SpawnActorDeferred(
        GetWorld(), 
        ProjectileClass,
        SpawnLocation, 
        SpawnRotation, 
        Owner
    );
    
    if (IsValid(CurrentProjectile))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Grenade Spawned Successfully"));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to Spawn Grenade"));
    }
}

void UCombatComponent::OnAimPressed(TSubclassOf<AProjectile> ProjectileClass)
{
    SpawnProjectile(ProjectileClass);
    Aim();
}

void UCombatComponent::OnThrowPressed()
{
    if (!IsValid(CurrentProjectile) || !GetWorld()) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No projectile to throw!"));
        return;
    }

    AActor* Owner = GetOwner();
    if (!IsValid(Owner)) return;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Throwing Grenade!"));

    // Get camera direction
    APlayerController* PlayerController = Cast<APlayerController>(Owner->GetInstigatorController());
    FVector CameraDirection = Owner->GetActorForwardVector();
    
    if (PlayerController && PlayerController->PlayerCameraManager)
    {
        CameraDirection = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
    }
    
    UFps_BlueprintFunctionLibrary::FinishSpawn(
        CurrentProjectile, 
        CurrentProjectile->GetActorLocation(), 
        CurrentProjectile->GetActorRotation(), 
        CameraDirection, 
        1.0f
    );
    
    CurrentProjectile->SetProjectileMovement(CachedLaunchVelocity);
    FTimerHandle DamageTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        DamageTimerHandle,
        [this]()
        {
            if (IsValid(CurrentProjectile))
            {
                CurrentProjectile->DealDamage();
            }
        },
        3.0f,
        false
    );

    ClearTrajectory();
    CurrentProjectile = nullptr;
}

void UCombatComponent::Aim()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Projectile: Aiming"));
    
    AActor* Owner = GetOwner();
    if (!Owner || !GetWorld()) return;

    // Get camera direction
    APlayerController* PlayerController = Cast<APlayerController>(Owner->GetInstigatorController());
    if (!PlayerController || !PlayerController->PlayerCameraManager) return;

    const FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
    FVector LaunchDirection = CameraRotation.Vector();
    
    // Calculate spawn location in front of player
    FVector SpawnLocation = Owner->GetActorLocation() + 
                           (Owner->GetActorForwardVector() * 100.0f) + 
                           FVector(0, 0, 50.0f); // Slightly above player

    // Predict trajectory
    PredictTrajectory(SpawnLocation, LaunchDirection);
    TrajectorySpline->SetVisibility(true);
}

void UCombatComponent::PredictTrajectory(const FVector& StartLocation, const FVector& LaunchDirection)
{
    if (!GetWorld() || !TrajectorySpline) return;
    
    FPredictProjectilePathParams PredictParams;
    PredictParams.StartLocation = StartLocation;
    PredictParams.LaunchVelocity = LaunchDirection * 1500.0f; // Adjust speed as needed
    PredictParams.ProjectileRadius = 10.0f;
    PredictParams.MaxSimTime = 3.0f;
    PredictParams.bTraceWithCollision = true;
    PredictParams.bTraceWithChannel = true; // filters out channels u dont need
    PredictParams.TraceChannel = ECC_WorldStatic; //trace against all objects that block the WorldStatic collision channel — e.g. walls, floors, and terrain.
    PredictParams.SimFrequency = 15.0f;
    PredictParams.OverrideGravityZ = 0.0f; 

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());
    PredictParams.ActorsToIgnore = ActorsToIgnore; // ignore our player

    PredictParams.LaunchVelocity = LaunchDirection * 1500.0f;
    CachedLaunchVelocity = PredictParams.LaunchVelocity;
    bool bHit = UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);
    
    if (PredictResult.PathData.Num() > 0)
    {
        for (const FPredictProjectilePathPointData& Point : PredictResult.PathData)
        {
            TrajectorySpline->AddSplinePoint(Point.Location, ESplineCoordinateSpace::World, true);
        }
        const int32 NumPointsToShow = 10;
        const int32 TotalPoints = PredictResult.PathData.Num();
        TrajectorySpline->UpdateSpline();
        
        int32 Step = FMath::Max(1, TotalPoints / NumPointsToShow);

        for (int32 i = 0; i < TotalPoints; i += Step)
        {
            DrawDebugSphere(
                GetWorld(),
                PredictResult.PathData[i].Location,
                8.0f,      
                12,         
                FColor::Green,
                false,   
                2.0f         
            );
        }
    }
}

void UCombatComponent::ClearTrajectory()
{
    if (TrajectorySpline)
    {
        TrajectorySpline->ClearSplinePoints();
        TrajectorySpline->SetVisibility(false);
    }
}