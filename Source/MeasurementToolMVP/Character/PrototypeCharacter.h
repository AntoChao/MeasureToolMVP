#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PrototypeCharacter.generated.h"

class UCameraComponent;

UCLASS()
class MEASUREMENTTOOLMVP_API APrototypeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APrototypeCharacter();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Custom|Input|Navigation")
	void ReceiveMoveInput(const FVector2D& MoveInput);

	UFUNCTION(BlueprintCallable, Category = "Custom|Input|Navigation")
	void ReceiveVerticalMoveInput(float VerticalInput);

	UFUNCTION(BlueprintCallable, Category = "Custom|Input|Navigation")
	void ReceiveLookInput(const FVector2D& LookInput);

	UFUNCTION(BlueprintCallable, Category = "Custom|Input|Navigation")
	void StopNavigationMovement();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom|Values|Movement", meta = (ClampMin = "0.0"))
	float FlyingSpeed = 1200.0f;

private:
	static constexpr float DefaultMaxAcceleration = 4000.0f;
	static constexpr float DefaultBrakingDecelerationFlying = 8000.0f;
};
