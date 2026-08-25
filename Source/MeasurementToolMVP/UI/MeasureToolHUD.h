#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeasureTool/MeasureToolTypes.h"
#include "MeasureToolHUD.generated.h"

class APrototypePlayerController;
class UButton;
class UCanvasPanel;
class UMeasureTool;
class UTextBlock;

UCLASS()
class MEASUREMENTTOOLMVP_API UMeasureToolHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void InitializeMeasureToolHUD(APrototypePlayerController* InPlayerController, UMeasureTool* InMeasureTool);
	bool IsPointerOverMeasureToolControls() const;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UCanvasPanel> canvas_root;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UButton> button_measureToolToggle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UTextBlock> txtBlock_measureToolToggle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UTextBlock> txtBlock_measureToolPointA;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UTextBlock> txtBlock_measureToolPointB;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UTextBlock> txtBlock_measureToolDistance;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UButton> button_measureToolClear;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UTextBlock> txtBlock_measureToolClear;

private:
	UFUNCTION()
	void HandleMeasureToolToggleClicked();

	UFUNCTION()
	void HandleMeasureToolClearClicked();

	void HandleMeasureToolChanged(const FMeasureToolSnapshot& Snapshot);
	void RefreshMeasureToolHUD();
	void ApplyMeasureToolHUDSnapshot(const FMeasureToolSnapshot& Snapshot);
	static FText FormatMeasureToolPoint(const FText& PointLabel, const FVector& Point);

	UPROPERTY(Transient)
	TObjectPtr<APrototypePlayerController> PlayerController;

	UPROPERTY(Transient)
	TObjectPtr<UMeasureTool> MeasureTool;

	FDelegateHandle MeasureToolChangedHandle;
};
