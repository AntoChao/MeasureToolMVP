#include "UI/MeasureToolHUD.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MeasureTool/MeasureTool.h"
#include "Player/PrototypePlayerController.h"

void UMeasureToolHUD::NativeConstruct()
{
	Super::NativeConstruct();

	button_measureToolToggle->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleMeasureToolToggleClicked);
	button_measureToolClear->OnClicked.AddUniqueDynamic(this, &ThisClass::HandleMeasureToolClearClicked);
	RefreshMeasureToolHUD();
}

void UMeasureToolHUD::NativeDestruct()
{
	if (IsValid(MeasureTool) && MeasureToolChangedHandle.IsValid())
	{
		MeasureTool->OnMeasureToolChanged().Remove(MeasureToolChangedHandle);
	}
	MeasureToolChangedHandle.Reset();

	Super::NativeDestruct();
}

void UMeasureToolHUD::InitializeMeasureToolHUD(
	APrototypePlayerController* InPlayerController,
	UMeasureTool* InMeasureTool)
{
	if (IsValid(MeasureTool) && MeasureToolChangedHandle.IsValid())
	{
		MeasureTool->OnMeasureToolChanged().Remove(MeasureToolChangedHandle);
	}

	PlayerController = InPlayerController;
	MeasureTool = InMeasureTool;
	MeasureToolChangedHandle.Reset();

	if (IsValid(MeasureTool))
	{
		MeasureToolChangedHandle = MeasureTool->OnMeasureToolChanged().AddUObject(
			this,
			&ThisClass::HandleMeasureToolChanged);
	}

	RefreshMeasureToolHUD();
}

bool UMeasureToolHUD::IsPointerOverMeasureToolControls() const
{
	return (button_measureToolToggle != nullptr && button_measureToolToggle->IsHovered())
		|| (button_measureToolClear != nullptr && button_measureToolClear->IsHovered());
}

void UMeasureToolHUD::HandleMeasureToolToggleClicked()
{
	if (IsValid(PlayerController))
	{
		PlayerController->ToggleMeasureTool();
	}
}

void UMeasureToolHUD::HandleMeasureToolClearClicked()
{
	if (IsValid(PlayerController))
	{
		PlayerController->ClearMeasureToolPoints();
	}
}

void UMeasureToolHUD::HandleMeasureToolChanged(const FMeasureToolSnapshot& Snapshot)
{
	ApplyMeasureToolHUDSnapshot(Snapshot);
}

void UMeasureToolHUD::RefreshMeasureToolHUD()
{
	ApplyMeasureToolHUDSnapshot(IsValid(MeasureTool)
		? MeasureTool->GetMeasureToolSnapshot()
		: FMeasureToolSnapshot());
}

void UMeasureToolHUD::ApplyMeasureToolHUDSnapshot(const FMeasureToolSnapshot& Snapshot)
{
	const bool bMeasureToolEnabled = Snapshot.State != EMeasureToolState::Disabled;
	txtBlock_measureToolToggle->SetText(bMeasureToolEnabled
		? NSLOCTEXT("MeasureTool", "DisableMeasure", "Disable Measure")
		: NSLOCTEXT("MeasureTool", "EnableMeasure", "Enable Measure"));

	txtBlock_measureToolPointA->SetText(Snapshot.bHasPointA
		? FormatMeasureToolPoint(NSLOCTEXT("MeasureTool", "PointALabel", "Point A"), Snapshot.PointA)
		: NSLOCTEXT("MeasureTool", "PointANotAvailable", "Point A: N/A"));
	txtBlock_measureToolPointB->SetText(Snapshot.bHasPointB
		? FormatMeasureToolPoint(NSLOCTEXT("MeasureTool", "PointBLabel", "Point B"), Snapshot.PointB)
		: NSLOCTEXT("MeasureTool", "PointBNotAvailable", "Point B: N/A"));

	const ESlateVisibility InformationVisibility = bMeasureToolEnabled
		? ESlateVisibility::HitTestInvisible
		: ESlateVisibility::Collapsed;
	txtBlock_measureToolPointA->SetVisibility(InformationVisibility);
	txtBlock_measureToolPointB->SetVisibility(InformationVisibility);

	txtBlock_measureToolDistance->SetVisibility(Snapshot.bHasPointB
		? ESlateVisibility::HitTestInvisible
		: ESlateVisibility::Collapsed);
	txtBlock_measureToolDistance->SetText(Snapshot.DistanceText);

	button_measureToolClear->SetVisibility(Snapshot.bHasPointA
		? ESlateVisibility::Visible
		: ESlateVisibility::Collapsed);
	txtBlock_measureToolClear->SetText(NSLOCTEXT("MeasureTool", "ClearMeasure", "Clear"));
}

FText UMeasureToolHUD::FormatMeasureToolPoint(const FText& PointLabel, const FVector& Point)
{
	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = 1;
	NumberFormat.MaximumFractionalDigits = 1;

	return FText::Format(
		NSLOCTEXT("MeasureTool", "PointFormat", "{0}: X {1}  Y {2}  Z {3}"),
		PointLabel,
		FText::AsNumber(Point.X, &NumberFormat),
		FText::AsNumber(Point.Y, &NumberFormat),
		FText::AsNumber(Point.Z, &NumberFormat));
}
