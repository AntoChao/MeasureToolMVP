#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PrototypeGameMode.generated.h"

UCLASS()
class MEASUREMENTTOOLMVP_API APrototypeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APrototypeGameMode();

	virtual void RestartPlayer(AController* NewPlayer) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Values|Spawn")
	FVector StartingCameraLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Values|Spawn")
	FRotator StartingCameraRotation = FRotator::ZeroRotator;
};
