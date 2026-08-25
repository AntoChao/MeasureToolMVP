#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeasureToolDistanceWidget.generated.h"

class UTextBlock;

UCLASS()
class MEASUREMENTTOOLMVP_API UMeasureToolDistanceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool|Widget")
	void SetMeasureToolDistanceText(const FText& DistanceText);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Custom|Bindings")
	TObjectPtr<UTextBlock> txtBlock_measureToolDistance;
};
