#pragma once

#include "CoreMinimal.h"
#include "MeasureToolTypes.generated.h"

UENUM(BlueprintType)
enum class EMeasureToolState : uint8
{
	Disabled,
	WaitingForFirstPoint,
	WaitingForSecondPoint,
	Complete
};

USTRUCT(BlueprintType)
struct MEASUREMENTTOOLMVP_API FMeasureToolSnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Measure Tool|Snapshot")
	EMeasureToolState State = EMeasureToolState::Disabled;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Measure Tool|Snapshot")
	bool bHasPointA = false;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Measure Tool|Snapshot")
	bool bHasPointB = false;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Measure Tool|Snapshot")
	FVector PointA = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Measure Tool|Snapshot")
	FVector PointB = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Measure Tool|Snapshot")
	double DistanceCentimeters = 0.0;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|Measure Tool|Snapshot")
	FText DistanceText;
};
