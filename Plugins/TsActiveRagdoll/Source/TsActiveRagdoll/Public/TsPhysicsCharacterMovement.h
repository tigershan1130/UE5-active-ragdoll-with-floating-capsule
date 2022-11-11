// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/ShapeComponent.h"
#include "PhysicsEngine/BodyInstance.h" // Add this include
#include "Components/CapsuleComponent.h"
#include "TsPhysicsCharacterMovement.generated.h"



UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class TSACTIVERAGDOLL_API UTsPhysicsCharacterMovement : public UPawnMovementComponent
{
	GENERATED_BODY()


		UTsPhysicsCharacterMovement(const FObjectInitializer& ObjectInitializer);

	//Begin UActorComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//End UActorComponent Interface

	// Custom physics Delegate
	FCalculateCustomPhysics OnCalculateCustomPhysics;
	void CustomPhysics(float DeltaTime, FBodyInstance* BodyInstance);


	//Begin UMovementComponent Interface
	virtual float GetMaxSpeed() const override { return MaxSpeed; }

protected:
	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;
public:

	// Event called every physics tick and sub-step.
	UFUNCTION(BlueprintNativeEvent)
		void PhysicsTick(float SubstepDeltaTime);
	virtual void PhysicsTick_Implementation(float SubstepDeltaTime);

	/** Maximum velocity magnitude allowed for the controlled Pawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement)
		float MaxSpeed;

	/** Acceleration applied by input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement)
		float Acceleration;

	/** Deceleration applied when there is no input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement)
		float Deceleration;


	/** Deceleration applied when there is no input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement)
		float RideSpringStrength;


	/** Deceleration applied when there is no input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement)
		float RideSpringDamper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement)
		float RideHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement)
		bool UseSubStep = true;

	// offset our visual mesh from our actual capsule collider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement)
		float MeshHeightOffset = 0;

	/**
	 * Setting affecting extra force applied when changing direction, making turns have less drift and become more responsive.
	 * Velocity magnitude is not allowed to increase, that only happens due to normal acceleration. It may decrease with large direction changes.
	 * Larger values apply extra force to reach the target direction more quickly, while a zero value disables any extra turn force.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PhysicsPawnMovement, meta = (ClampMin = "0", UIMin = "0"))
		float TurningBoost;

	UPROPERTY()
		UShapeComponent* RootDriveComponent;

	UPROPERTY()
		USkeletalMeshComponent* CurrentSkeletalMeshComponent;

	UFUNCTION(BlueprintCallable, Category = PhysicsPawnMovement)
		void InitializeCapsuleBasedMovement(UCapsuleComponent* CapsuleComponent, USkeletalMeshComponent* SkeletalMeshComp);


	UFUNCTION(BlueprintCallable, Category = PhysicsPawnMovement)
		FRotator GetPhysicsRotation();



protected:

	/** Update Velocity based on input. Also applies gravity. */
	virtual void ApplyControlInputToVelocity(float DeltaTime);

	/** Prevent Pawn from leaving the world bounds (if that restriction is enabled in WorldSettings) */
	virtual bool LimitWorldBounds();

	/** Set to true when a position correction is applied. Used to avoid recalculating velocity when this occurs. */
	UPROPERTY(Transient)
		uint32 bPositionCorrected : 1;

	UPROPERTY()
		uint32 InitDriveType = 0;

	UPROPERTY()
		bool IsGrounded = true;
};
