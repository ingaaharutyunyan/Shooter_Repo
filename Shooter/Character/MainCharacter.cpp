#include "MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../Components/CombatComponent.h"
#include "Engine/LocalPlayer.h"
#include "Shooter/Weapons/Grenade.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMainCharacter::AMainCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
        
    // Create a CameraComponent
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

    CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}

void AMainCharacter::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);

        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMainCharacter::Aim);
        EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Completed, this, &AMainCharacter::Throw);
    }
    else
    {
        UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Missing Enhanced Input Component!"), *GetNameSafe(this));
    }
}
void AMainCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller)
    {
        const FRotator ControlRotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, ControlRotation.Yaw, 0);

        const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector Right   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(Forward, MovementVector.Y);
        AddMovementInput(Right, MovementVector.X);

        UE_LOG(LogTemp, Warning, TEXT("MoveAxis: X=%f, Y=%f"), MovementVector.X, MovementVector.Y);
    }
}


void AMainCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller)
    {
        AddControllerYawInput(LookAxisVector.X * 0.3f);
        AddControllerPitchInput(LookAxisVector.Y * 0.3f);
    }
}

void AMainCharacter::Aim(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Called OnAimPressed from maincharacter"));
    CombatComponent->OnAimPressed(AGrenade::StaticClass());
}

void AMainCharacter::Throw(const FInputActionValue& Value)
{
    CombatComponent->OnThrowPressed();
}

void AMainCharacter::UpdateHealth(float DeltaHealth)
{
    float OldHealth = hp;
    hp = FMath::Clamp(hp + DeltaHealth, 0.0f, 100.0f);

    if (OldHealth != hp)
    {
        OnHealthChanged.Broadcast(hp);
    }
}
