#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeasureTool/MeasureToolTypes.h"
#include "MeasureVisualActor.generated.h"

class UMaterialInstanceDynamic;
class UMaterialInterface;
class USceneComponent;
class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class MEASUREMENTTOOLMVP_API AMeasureVisualActor : public AActor
{
	GENERATED_BODY()

public:
	AMeasureVisualActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool|Visual")
	void ApplyMeasureToolSnapshot(const FMeasureToolSnapshot& Snapshot);

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool|Visual")
	void ClearMeasureToolVisuals();

	UFUNCTION(BlueprintCallable, Category = "Custom|Measure Tool|Visual")
	void SetMeasureToolVisualStyle();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	TObjectPtr<UStaticMeshComponent> PointAIndicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	TObjectPtr<UStaticMeshComponent> PointBIndicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	TObjectPtr<UStaticMeshComponent> MeasureLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	TObjectPtr<UWidgetComponent> DistanceWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Materials")
	TObjectPtr<UMaterialInterface> IndicatorMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Materials|Points")
	FLinearColor PointColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Materials|Points", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PointOpacity = 0.85f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Materials|Line")
	FLinearColor LineColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Materials|Line", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LineOpacity = 0.85f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Materials|Parameters")
	FName MeasureToolColorParameterName = TEXT("BasicColor");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Materials|Parameters")
	FName MeasureToolOpacityParameterName = TEXT("Opacity");

private:
	void ConfigureMeasureToolMesh(UStaticMeshComponent* MeshComponent) const;
	void UpdateMeasureToolWidgetFacing();

	static constexpr float DefaultCylinderLengthCentimeters = 100.0f;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> PointAMaterial;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> PointBMaterial;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> LineMaterial;

	float MeasureToolCylinderLengthCentimeters = DefaultCylinderLengthCentimeters;
};
