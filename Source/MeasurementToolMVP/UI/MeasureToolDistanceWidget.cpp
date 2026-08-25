#include "UI/MeasureToolDistanceWidget.h"

#include "Components/TextBlock.h"

void UMeasureToolDistanceWidget::SetMeasureToolDistanceText(const FText& DistanceText)
{
	if (txtBlock_measureToolDistance != nullptr)
	{
		txtBlock_measureToolDistance->SetText(DistanceText);
	}
}
