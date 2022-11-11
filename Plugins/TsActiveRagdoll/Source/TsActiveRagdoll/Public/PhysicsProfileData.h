// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "PhysicsProfileData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct TSACTIVERAGDOLL_API FPhysicsProfileData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlendTorqueWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BlendForceWeight;
};


USTRUCT(BlueprintType)
struct TSACTIVERAGDOLL_API FPhysicsProfileState : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPhysicsProfileData> ProfileSettings;
};
