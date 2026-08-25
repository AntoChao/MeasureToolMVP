#include "Player/PrototypePlayerController.h"

#include "Character/PrototypeCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "MeasureTool/MeasureTool.h"
#include "MeasureTool/MeasureVisualActor.h"
#include "UI/MeasureToolHUD.h"

APrototypePlayerController::APrototypePlayerController()
{
	MeasureTool = CreateDefaultSubobject<UMeasureTool>(TEXT("MeasureTool"));
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APrototypePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	AddNavigationInputContext();

	if (MeasureToolHUDClass != nullptr)
	{
		MeasureToolHUD = CreateWidget<UMeasureToolHUD>(this, MeasureToolHUDClass);
		if (IsValid(MeasureToolHUD))
		{
			MeasureToolHUD->InitializeMeasureToolHUD(this, MeasureTool);
			MeasureToolHUD->AddToViewport();
		}
	}
}

void APrototypePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (MoveInputAction != nullptr)
	{
		EnhancedInput->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMove);
	}
	if (VerticalMoveInputAction != nullptr)
	{
		EnhancedInput->BindAction(VerticalMoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleVerticalMove);
	}
	if (LookInputAction != nullptr)
	{
		EnhancedInput->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLook);
	}
	if (NavigationHoldInputAction != nullptr)
	{
		EnhancedInput->BindAction(NavigationHoldInputAction, ETriggerEvent::Started, this, &ThisClass::HandleNavigationStarted);
		EnhancedInput->BindAction(NavigationHoldInputAction, ETriggerEvent::Completed, this, &ThisClass::HandleNavigationStopped);
		EnhancedInput->BindAction(NavigationHoldInputAction, ETriggerEvent::Canceled, this, &ThisClass::HandleNavigationStopped);
	}
	if (MeasureToolSelectPointInputAction != nullptr)
	{
		EnhancedInput->BindAction(MeasureToolSelectPointInputAction, ETriggerEvent::Started, this, &ThisClass::HandleMeasureToolSelectPoint);
	}
}

void APrototypePlayerController::EnableMeasureTool()
{
	SetMeasureToolInputContextEnabled(true);
	MeasureTool->EnableMeasureTool();
}

void APrototypePlayerController::DisableMeasureTool()
{
	SetMeasureToolInputContextEnabled(false);
	MeasureTool->DisableMeasureTool();
}

void APrototypePlayerController::ToggleMeasureTool()
{
	if (MeasureTool->IsMeasureToolEnabled())
	{
		DisableMeasureTool();
	}
	else
	{
		EnableMeasureTool();
	}
}

void APrototypePlayerController::ClearMeasureToolPoints()
{
	MeasureTool->ClearMeasureToolPoints();
}

bool APrototypePlayerController::TraceMeasureToolPointUnderCursor(FHitResult& OutHitResult) const
{
	FVector TraceStart;
	FVector TraceDirection;
	if (!DeprojectMousePositionToWorld(TraceStart, TraceDirection) || GetWorld() == nullptr)
	{
		return false;
	}

	FCollisionQueryParams TraceParameters(SCENE_QUERY_STAT(MeasureToolTrace), true);
	TraceParameters.bReturnPhysicalMaterial = false;
	if (GetPawn() != nullptr)
	{
		TraceParameters.AddIgnoredActor(GetPawn());
	}
	if (const AMeasureVisualActor* MeasureVisualActor = MeasureTool->GetMeasureVisualActor())
	{
		TraceParameters.AddIgnoredActor(MeasureVisualActor);
	}

	const FVector TraceEnd = TraceStart + TraceDirection * MeasureToolTraceDistance;
	return GetWorld()->LineTraceSingleByChannel(
		OutHitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		TraceParameters) && OutHitResult.IsValidBlockingHit();
}

UMeasureTool* APrototypePlayerController::GetMeasureTool() const
{
	return MeasureTool;
}

void APrototypePlayerController::AddNavigationInputContext()
{
	if (NavigationMappingContext == nullptr)
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(NavigationMappingContext, 0);
		}
	}
}

void APrototypePlayerController::SetMeasureToolInputContextEnabled(const bool bEnabled)
{
	if (MeasureToolMappingContext == nullptr || bMeasureToolInputContextApplied == bEnabled)
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (bEnabled)
			{
				InputSubsystem->AddMappingContext(MeasureToolMappingContext, 10);
			}
			else
			{
				InputSubsystem->RemoveMappingContext(MeasureToolMappingContext);
			}
			bMeasureToolInputContextApplied = bEnabled;
		}
	}
}

void APrototypePlayerController::HandleMove(const FInputActionValue& InputValue)
{
	if (bNavigationHeld)
	{
		if (APrototypeCharacter* ControlledCharacter = Cast<APrototypeCharacter>(GetPawn()))
		{
			ControlledCharacter->ReceiveMoveInput(InputValue.Get<FVector2D>());
		}
	}
}

void APrototypePlayerController::HandleVerticalMove(const FInputActionValue& InputValue)
{
	if (bNavigationHeld)
	{
		if (APrototypeCharacter* ControlledCharacter = Cast<APrototypeCharacter>(GetPawn()))
		{
			ControlledCharacter->ReceiveVerticalMoveInput(InputValue.Get<float>());
		}
	}
}

void APrototypePlayerController::HandleLook(const FInputActionValue& InputValue)
{
	if (bNavigationHeld)
	{
		if (APrototypeCharacter* ControlledCharacter = Cast<APrototypeCharacter>(GetPawn()))
		{
			ControlledCharacter->ReceiveLookInput(InputValue.Get<FVector2D>() * CameraLookSpeed);
		}
	}
}

void APrototypePlayerController::HandleNavigationStarted()
{
	bNavigationHeld = true;
}

void APrototypePlayerController::HandleNavigationStopped()
{
	bNavigationHeld = false;
	if (APrototypeCharacter* ControlledCharacter = Cast<APrototypeCharacter>(GetPawn()))
	{
		ControlledCharacter->StopNavigationMovement();
	}
}

void APrototypePlayerController::HandleMeasureToolSelectPoint()
{
	if (!MeasureTool->IsMeasureToolEnabled()
		|| bNavigationHeld
		|| (IsValid(MeasureToolHUD) && MeasureToolHUD->IsPointerOverMeasureToolControls()))
	{
		return;
	}

	FHitResult SurfaceHit;
	if (TraceMeasureToolPointUnderCursor(SurfaceHit))
	{
		MeasureTool->SubmitMeasureToolSurfaceHit(SurfaceHit);
	}
}
