#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PrototypePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UMeasureTool;
class UMeasureToolHUD;
struct FInputActionValue;

UCLASS()
class MEASUREMENTTOOLMVP_API APrototypePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APrototypePlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	void EnableMeasureTool();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	void DisableMeasureTool();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	void ToggleMeasureTool();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	void ClearMeasureToolPoints();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool|Trace")
	bool TraceMeasureToolPointUnderCursor(FHitResult& OutHitResult) const;

	UFUNCTION(BlueprintPure, Category = "Custom|Measure Tool")
	UMeasureTool* GetMeasureTool() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	TObjectPtr<UMeasureTool> MeasureTool;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input|Navigation")
	TObjectPtr<UInputMappingContext> NavigationMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input|Navigation")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input|Navigation")
	TObjectPtr<UInputAction> VerticalMoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input|Navigation")
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input|Navigation")
	TObjectPtr<UInputAction> NavigationHoldInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input|Measure Tool")
	TObjectPtr<UInputMappingContext> MeasureToolMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Input|Measure Tool")
	TObjectPtr<UInputAction> MeasureToolSelectPointInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|HUD")
	TSubclassOf<UMeasureToolHUD> MeasureToolHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Trace", meta = (ClampMin = "1.0", Units = "cm"))
	float MeasureToolTraceDistance = 100000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Values|Camera", meta = (ClampMin = "0.0"))
	float CameraLookSpeed = 1.0f;

private:
	void AddNavigationInputContext();
	void SetMeasureToolInputContextEnabled(bool bEnabled);
	void HandleMove(const FInputActionValue& InputValue);
	void HandleVerticalMove(const FInputActionValue& InputValue);
	void HandleLook(const FInputActionValue& InputValue);
	void HandleNavigationStarted();
	void HandleNavigationStopped();
	void HandleMeasureToolSelectPoint();

	UPROPERTY(Transient)
	TObjectPtr<UMeasureToolHUD> MeasureToolHUD;

	bool bNavigationHeld = false;
	bool bMeasureToolInputContextApplied = false;
};
