// This is a Floating Capsule Physics Movement Character
// Copyright @ Tiger Shan 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "TsPhysicsCharacterMovement.h"
#include "TsPhysicsCharacter.generated.h"

UCLASS()
class TSACTIVERAGDOLL_API ATsPhysicsCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATsPhysicsCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTsPhysicsCharacterMovement* PhysicsCharacterMovement;

	UPROPERTY(EditAnywhere)
		float MoveScale = 1;

	/**
	 * @brief Handle input to update location
	 * @param ActionValue
	*/
	void Move(const struct FInputActionValue& ActionValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
