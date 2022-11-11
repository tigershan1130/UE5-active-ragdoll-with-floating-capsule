#pragma once

#include "Engine.h"
#include "Animation/AnimNodeBase.h"
#include "AnimNode_ActiveRagdoll.generated.h"



USTRUCT(BlueprintType)
struct TSACTIVERAGDOLL_API FAnimNode_ActiveRagdoll : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
		FComponentSpacePoseLink mBasePose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links, meta = (PinShownByDefault))
		mutable FRotator mInitialRelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links, meta = (PinShownByDefault))
		mutable FVector mInitialRelativeTranslation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Links, meta = (PinShownByDefault))
		mutable float mTranslationWeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Links, meta = (PinShownByDefault))
		mutable float mRotationWeight;

protected:
	float mDeltaTime;

public:
	/************/
	FAnimNode_ActiveRagdoll();

	/****************************************************************/
	void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;

	/****************************************************************/
	void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	/****************************************************************/
	void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;

	/*********************************************************/
	void EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output) override;

	// update physics stufff
	void PhysicsTick(float SubstepDeltaTime, FComponentSpacePoseContext& Output);
};
