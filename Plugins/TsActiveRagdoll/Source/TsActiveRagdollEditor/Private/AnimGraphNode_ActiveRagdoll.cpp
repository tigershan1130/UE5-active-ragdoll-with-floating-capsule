// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimGraphNode_ActiveRagdoll.h"
#include "AnimationGraphSchema.h"
#include "AnimNode_ActiveRagdoll.h"
#include "Animation/AnimInstanceProxy.h"




/******************Title Color!****************************/
FLinearColor UAnimGraphNode_ActiveRagdoll::GetNodeTitleColor() const
{
	return FLinearColor(0, 12.0f, 12.0f, 1.0f);
}

/***************Node Category***********************/
FString UAnimGraphNode_ActiveRagdoll::GetNodeCategory() const
{
	return FString("Active Ragdoll");
}

/*******************************Node Title************************************/
FText UAnimGraphNode_ActiveRagdoll::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Move Physical Bones to Animation Pose");
}


/*******************************Exposing Output Pins************************************/
void UAnimGraphNode_ActiveRagdoll::CreateOutputPins()
{
	const UAnimationGraphSchema* Schema = GetDefault<UAnimationGraphSchema>();
	CreatePin(EGPD_Output, Schema->PC_Struct, TEXT(""), FComponentSpacePoseLink::StaticStruct(), /*bIsArray=*/ false, /*bIsReference=*/ false, TEXT("Pose"));
}

