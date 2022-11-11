// Fill out your copyright notice in the Description page of Project Settings.


#include "TsPlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"

void ATsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// create these objects here and not in constructor because we only need thyem on the client
	PawnMappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;
	PawnMappingContext->MapKey(MoveAction, EKeys::W);



}