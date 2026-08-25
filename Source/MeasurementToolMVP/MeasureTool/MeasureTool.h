#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeasureTool/MeasureToolTypes.h"
#include "MeasureTool.generated.h"

class AMeasureVisualActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMeasureToolChanged, const FMeasureToolSnapshot&);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MEASUREMENTTOOLMVP_API UMeasureTool : public UActorComponent
{
	GENERATED_BODY()

public:
	UMeasureTool();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	void EnableMeasureTool();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	void DisableMeasureTool();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	void ToggleMeasureTool();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	void ClearMeasureToolPoints();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool")
	bool SubmitMeasureToolSurfaceHit(const FHitResult& SurfaceHit);

	UFUNCTION(BlueprintPure, Category = "Custom|Measure Tool")
	bool IsMeasureToolEnabled() const;

	UFUNCTION(BlueprintPure, Category = "Custom|Measure Tool")
	FMeasureToolSnapshot GetMeasureToolSnapshot() const;

	AMeasureVisualActor* GetMeasureVisualActor() const;
	FOnMeasureToolChanged& OnMeasureToolChanged();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Visual")
	TSubclassOf<AMeasureVisualActor> MeasureVisualActorClass;

private:
	void BeginNewMeasureToolPoint(const FVector& SurfacePoint);
	void CompleteMeasureToolPoint(const FVector& SurfacePoint);
	void EnsureMeasureVisualActor();
	void DestroyMeasureVisualActor();
	void PublishMeasureToolSnapshot();

	EMeasureToolState MeasureToolState = EMeasureToolState::Disabled;
	bool bHasMeasureToolPointA = false;
	bool bHasMeasureToolPointB = false;
	FVector MeasureToolPointA = FVector::ZeroVector;
	FVector MeasureToolPointB = FVector::ZeroVector;
	double MeasureToolDistanceCentimeters = 0.0;

	UPROPERTY(Transient)
	TObjectPtr<AMeasureVisualActor> MeasureVisualActor;

	FOnMeasureToolChanged MeasureToolChanged;
};
