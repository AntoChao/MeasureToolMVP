#include "MeasureTool/MeasureVisualActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/MeasureToolDistanceWidget.h"
#include "UObject/ConstructorHelpers.h"

AMeasureVisualActor::AMeasureVisualActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorEnableCollision(false);

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	PointAIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointAIndicator"));
	PointAIndicator->SetupAttachment(SceneRoot);
	ConfigureMeasureToolMesh(PointAIndicator);

	PointBIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointBIndicator"));
	PointBIndicator->SetupAttachment(SceneRoot);
	ConfigureMeasureToolMesh(PointBIndicator);

	MeasureLine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeasureLine"));
	MeasureLine->SetupAttachment(SceneRoot);
	ConfigureMeasureToolMesh(MeasureLine);

	DistanceWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DistanceWidget"));
	DistanceWidget->SetupAttachment(SceneRoot);
	DistanceWidget->SetWidgetSpace(EWidgetSpace::World);
	DistanceWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DistanceWidget->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		PointAIndicator->SetStaticMesh(SphereMesh.Object);
		PointBIndicator->SetStaticMesh(SphereMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(
		TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderMesh.Succeeded())
	{
		MeasureLine->SetStaticMesh(CylinderMesh.Object);
	}

	ClearMeasureToolVisuals();
}

void AMeasureVisualActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(IndicatorMaterial))
	{
		PointAIndicator->SetMaterial(0, IndicatorMaterial);
		PointBIndicator->SetMaterial(0, IndicatorMaterial);
		MeasureLine->SetMaterial(0, IndicatorMaterial);
	}

	if (UStaticMesh* CylinderMesh = MeasureLine->GetStaticMesh())
	{
		const float BoundsLength = CylinderMesh->GetBounds().BoxExtent.Z * 2.0f;
		if (BoundsLength > KINDA_SMALL_NUMBER)
		{
			MeasureToolCylinderLengthCentimeters = BoundsLength;
		}
	}

	PointAMaterial = PointAIndicator->CreateAndSetMaterialInstanceDynamic(0);
	PointBMaterial = PointBIndicator->CreateAndSetMaterialInstanceDynamic(0);
	LineMaterial = MeasureLine->CreateAndSetMaterialInstanceDynamic(0);
	SetMeasureToolVisualStyle();

	DistanceWidget->InitWidget();
}

void AMeasureVisualActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateMeasureToolWidgetFacing();
}

void AMeasureVisualActor::ApplyMeasureToolSnapshot(const FMeasureToolSnapshot& Snapshot)
{
	if (!Snapshot.bHasPointA)
	{
		ClearMeasureToolVisuals();
		return;
	}

	const FVector Midpoint = Snapshot.bHasPointB
		? (Snapshot.PointA + Snapshot.PointB) * 0.5
		: Snapshot.PointA;
	SetActorLocation(Midpoint);

	PointAIndicator->SetRelativeLocation(Snapshot.PointA - Midpoint);
	PointAIndicator->SetHiddenInGame(false);
	PointBIndicator->SetHiddenInGame(!Snapshot.bHasPointB);
	MeasureLine->SetHiddenInGame(!Snapshot.bHasPointB);
	DistanceWidget->SetHiddenInGame(!Snapshot.bHasPointB);
	SetActorTickEnabled(Snapshot.bHasPointB);

	if (!Snapshot.bHasPointB)
	{
		return;
	}

	PointBIndicator->SetRelativeLocation(Snapshot.PointB - Midpoint);

	const FVector MeasureDirection = Snapshot.PointB - Snapshot.PointA;
	const float MeasureDistance = MeasureDirection.Size();
	MeasureLine->SetRelativeLocation(FVector::ZeroVector);
	MeasureLine->SetRelativeRotation(FRotationMatrix::MakeFromZ(MeasureDirection.GetSafeNormal()).Rotator());
	const FVector AuthoredLineScale = MeasureLine->GetRelativeScale3D();
	MeasureLine->SetRelativeScale3D(FVector(
		AuthoredLineScale.X,
		AuthoredLineScale.Y,
		MeasureDistance / MeasureToolCylinderLengthCentimeters));

	DistanceWidget->InitWidget();
	if (UMeasureToolDistanceWidget* MeasureDistanceWidget =
		Cast<UMeasureToolDistanceWidget>(DistanceWidget->GetUserWidgetObject()))
	{
		MeasureDistanceWidget->SetMeasureToolDistanceText(Snapshot.DistanceText);
	}

	UpdateMeasureToolWidgetFacing();
}

void AMeasureVisualActor::ClearMeasureToolVisuals()
{
	PointAIndicator->SetHiddenInGame(true);
	PointBIndicator->SetHiddenInGame(true);
	MeasureLine->SetHiddenInGame(true);
	DistanceWidget->SetHiddenInGame(true);
	SetActorTickEnabled(false);
}

void AMeasureVisualActor::SetMeasureToolVisualStyle()
{
	const TArray<UMaterialInstanceDynamic*> PointMaterials =
	{
		PointAMaterial,
		PointBMaterial
	};

	for (UMaterialInstanceDynamic* Material : PointMaterials)
	{
		if (IsValid(Material))
		{
			Material->SetVectorParameterValue(MeasureToolColorParameterName, PointColor);
			Material->SetScalarParameterValue(MeasureToolOpacityParameterName, PointOpacity);
		}
	}

	if (IsValid(LineMaterial))
	{
		LineMaterial->SetVectorParameterValue(MeasureToolColorParameterName, LineColor);
		LineMaterial->SetScalarParameterValue(MeasureToolOpacityParameterName, LineOpacity);
	}
}

void AMeasureVisualActor::ConfigureMeasureToolMesh(UStaticMeshComponent* MeshComponent) const
{
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->SetCastShadow(false);
}

void AMeasureVisualActor::UpdateMeasureToolWidgetFacing()
{
	if (DistanceWidget->bHiddenInGame || GetWorld() == nullptr)
	{
		return;
	}

	if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (PlayerController->PlayerCameraManager != nullptr)
		{
			const FVector DirectionToCamera =
				PlayerController->PlayerCameraManager->GetCameraLocation() - DistanceWidget->GetComponentLocation();
			DistanceWidget->SetWorldRotation(DirectionToCamera.Rotation());
		}
	}
}
