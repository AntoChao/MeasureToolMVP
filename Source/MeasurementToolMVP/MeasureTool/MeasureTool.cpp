#include "MeasureTool/MeasureTool.h"

#include "Engine/World.h"
#include "MeasureTool/MeasureVisualActor.h"

UMeasureTool::UMeasureTool()
{
	PrimaryComponentTick.bCanEverTick = false;
	MeasureVisualActorClass = AMeasureVisualActor::StaticClass();
}

void UMeasureTool::OnComponentDestroyed(const bool bDestroyingHierarchy)
{
	DestroyMeasureVisualActor();
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UMeasureTool::EnableMeasureTool()
{
	if (IsMeasureToolEnabled())
	{
		return;
	}

	MeasureToolState = EMeasureToolState::WaitingForFirstPoint;
	bHasMeasureToolPointA = false;
	bHasMeasureToolPointB = false;
	MeasureToolDistanceCentimeters = 0.0;
	PublishMeasureToolSnapshot();
}

void UMeasureTool::DisableMeasureTool()
{
	MeasureToolState = EMeasureToolState::Disabled;
	bHasMeasureToolPointA = false;
	bHasMeasureToolPointB = false;
	MeasureToolDistanceCentimeters = 0.0;
	DestroyMeasureVisualActor();
	PublishMeasureToolSnapshot();
}

void UMeasureTool::ToggleMeasureTool()
{
	if (IsMeasureToolEnabled())
	{
		DisableMeasureTool();
	}
	else
	{
		EnableMeasureTool();
	}
}

void UMeasureTool::ClearMeasureToolPoints()
{
	bHasMeasureToolPointA = false;
	bHasMeasureToolPointB = false;
	MeasureToolPointA = FVector::ZeroVector;
	MeasureToolPointB = FVector::ZeroVector;
	MeasureToolDistanceCentimeters = 0.0;
	MeasureToolState = IsMeasureToolEnabled()
		? EMeasureToolState::WaitingForFirstPoint
		: EMeasureToolState::Disabled;
	DestroyMeasureVisualActor();
	PublishMeasureToolSnapshot();
}

bool UMeasureTool::SubmitMeasureToolSurfaceHit(const FHitResult& SurfaceHit)
{
	if (!IsMeasureToolEnabled() || !SurfaceHit.IsValidBlockingHit())
	{
		return false;
	}

	const FVector SurfacePoint = SurfaceHit.ImpactPoint;
	if (MeasureToolState == EMeasureToolState::WaitingForSecondPoint)
	{
		if (FVector::DistSquared(MeasureToolPointA, SurfacePoint) <= FMath::Square(KINDA_SMALL_NUMBER))
		{
			return false;
		}

		CompleteMeasureToolPoint(SurfacePoint);
	}
	else
	{
		BeginNewMeasureToolPoint(SurfacePoint);
	}

	EnsureMeasureVisualActor();
	PublishMeasureToolSnapshot();
	return true;
}

bool UMeasureTool::IsMeasureToolEnabled() const
{
	return MeasureToolState != EMeasureToolState::Disabled;
}

FMeasureToolSnapshot UMeasureTool::GetMeasureToolSnapshot() const
{
	FMeasureToolSnapshot Snapshot;
	Snapshot.State = MeasureToolState;
	Snapshot.bHasPointA = bHasMeasureToolPointA;
	Snapshot.bHasPointB = bHasMeasureToolPointB;
	Snapshot.PointA = MeasureToolPointA;
	Snapshot.PointB = MeasureToolPointB;
	Snapshot.DistanceCentimeters = MeasureToolDistanceCentimeters;

	if (bHasMeasureToolPointB)
	{
		FNumberFormattingOptions NumberFormat;
		NumberFormat.MinimumFractionalDigits = 2;
		NumberFormat.MaximumFractionalDigits = 2;
		Snapshot.DistanceText = FText::Format(
			NSLOCTEXT("MeasureTool", "DistanceCentimeters", "{0} cm"),
			FText::AsNumber(MeasureToolDistanceCentimeters, &NumberFormat));
	}

	return Snapshot;
}

AMeasureVisualActor* UMeasureTool::GetMeasureVisualActor() const
{
	return MeasureVisualActor;
}

FOnMeasureToolChanged& UMeasureTool::OnMeasureToolChanged()
{
	return MeasureToolChanged;
}

void UMeasureTool::BeginNewMeasureToolPoint(const FVector& SurfacePoint)
{
	MeasureToolPointA = SurfacePoint;
	MeasureToolPointB = FVector::ZeroVector;
	bHasMeasureToolPointA = true;
	bHasMeasureToolPointB = false;
	MeasureToolDistanceCentimeters = 0.0;
	MeasureToolState = EMeasureToolState::WaitingForSecondPoint;
}

void UMeasureTool::CompleteMeasureToolPoint(const FVector& SurfacePoint)
{
	MeasureToolPointB = SurfacePoint;
	bHasMeasureToolPointB = true;
	MeasureToolDistanceCentimeters = FVector::Distance(MeasureToolPointA, MeasureToolPointB);
	MeasureToolState = EMeasureToolState::Complete;
}

void UMeasureTool::EnsureMeasureVisualActor()
{
	if (IsValid(MeasureVisualActor) || GetWorld() == nullptr || MeasureVisualActorClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	MeasureVisualActor = GetWorld()->SpawnActor<AMeasureVisualActor>(
		MeasureVisualActorClass,
		FTransform::Identity,
		SpawnParameters);
}

void UMeasureTool::DestroyMeasureVisualActor()
{
	if (IsValid(MeasureVisualActor))
	{
		MeasureVisualActor->Destroy();
	}
	MeasureVisualActor = nullptr;
}

void UMeasureTool::PublishMeasureToolSnapshot()
{
	const FMeasureToolSnapshot Snapshot = GetMeasureToolSnapshot();
	if (IsValid(MeasureVisualActor))
	{
		MeasureVisualActor->ApplyMeasureToolSnapshot(Snapshot);
	}
	MeasureToolChanged.Broadcast(Snapshot);
}
