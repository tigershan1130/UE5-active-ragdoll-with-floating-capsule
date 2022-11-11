// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimGraphNode_Base.h"
#include "AnimNode_ActiveRagdoll.h"
#include "AnimGraphNode_ActiveRagdoll.generated.h"


UCLASS(BlueprintType)
class TSACTIVERAGDOLLEDITOR_API UAnimGraphNode_ActiveRagdoll : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Links)
		FAnimNode_ActiveRagdoll mAnimPose;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FString GetNodeCategory() const override;
	virtual void CreateOutputPins() override;



};


