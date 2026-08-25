#include "Character/PrototypeCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APrototypeCharacter::APrototypeCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetHiddenInGame(true);
	GetMesh()->SetVisibility(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetGenerateOverlapEvents(false);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector::ZeroVector);
	Camera->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->GravityScale = 0.0f;
	MovementComponent->MaxFlySpeed = FlyingSpeed;
	MovementComponent->MaxAcceleration = DefaultMaxAcceleration;
	MovementComponent->BrakingDecelerationFlying = DefaultBrakingDecelerationFlying;
	MovementComponent->DefaultLandMovementMode = MOVE_Flying;
}

void APrototypeCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxFlySpeed = FlyingSpeed;
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void APrototypeCharacter::ReceiveMoveInput(const FVector2D& MoveInput)
{
	if (Controller == nullptr || MoveInput.IsNearlyZero())
	{
		return;
	}

	const FRotator ControlRotation = Controller->GetControlRotation();
	AddMovementInput(ControlRotation.Vector(), MoveInput.X);
	AddMovementInput(FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y), MoveInput.Y);
}

void APrototypeCharacter::ReceiveVerticalMoveInput(const float VerticalInput)
{
	if (!FMath::IsNearlyZero(VerticalInput))
	{
		AddMovementInput(FVector::UpVector, VerticalInput);
	}
}

void APrototypeCharacter::ReceiveLookInput(const FVector2D& LookInput)
{
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(-LookInput.Y);
}

void APrototypeCharacter::StopNavigationMovement()
{
	GetCharacterMovement()->StopMovementImmediately();
}
