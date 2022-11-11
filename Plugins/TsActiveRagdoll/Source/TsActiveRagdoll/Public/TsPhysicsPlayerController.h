// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TsPhysicsPlayerController.generated.h"

/**
 *
 */
UCLASS()
class TSACTIVERAGDOLL_API ATsPhysicsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	* @brief Setup input actions and context mappings for player
	*/
	virtual void SetupInputComponent() override;

	/**
	 * @brief Mapping context used for pawn control
	*/
	UPROPERTY()
		class UInputMappingContext* PawnMappingContext;


	/**
	 * @brief Action to upate
	*/
	UPROPERTY()
		class UInputAction* MoveAction;
};