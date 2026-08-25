#include "Game/PrototypeGameMode.h"

#include "Character/PrototypeCharacter.h"
#include "Player/PrototypePlayerController.h"

APrototypeGameMode::APrototypeGameMode()
{
	DefaultPawnClass = APrototypeCharacter::StaticClass();
	PlayerControllerClass = APrototypePlayerController::StaticClass();
}

void APrototypeGameMode::RestartPlayer(AController* NewPlayer)
{
	if (!IsValid(NewPlayer))
	{
		return;
	}

	const FTransform SpawnTransform(StartingCameraRotation, StartingCameraLocation);
	RestartPlayerAtTransform(NewPlayer, SpawnTransform);
}
