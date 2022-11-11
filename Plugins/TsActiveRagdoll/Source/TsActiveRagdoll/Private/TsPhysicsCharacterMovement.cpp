// Fill out your copyright notice in the Description page of Project Settings.


#include "TsPhysicsCharacterMovement.h"
#include "DrawDebugHelpers.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)

UTsPhysicsCharacterMovement::UTsPhysicsCharacterMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxSpeed = 200.f;
	Acceleration = 4000.f;
	Deceleration = 400000.f;
	TurningBoost = 8.0f;
	RideHeight = 50.0f;
	RideSpringStrength = 300.0f;
	RideSpringDamper = 50.0f;

	bPositionCorrected = false;

	ResetMoveState();

	OnCalculateCustomPhysics.BindUObject(this, &UTsPhysicsCharacterMovement::CustomPhysics);
}

void UTsPhysicsCharacterMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (InitDriveType == 0)
		return;

	if (InitDriveType == 1 && !RootDriveComponent)
		return;

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}


	if (UseSubStep)
	{
		if (RootDriveComponent->GetBodyInstance())
			RootDriveComponent->GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
	}
	else
	{
		PhysicsTick(DeltaTime);
	}
}



bool UTsPhysicsCharacterMovement::LimitWorldBounds()
{
	AWorldSettings* WorldSettings = PawnOwner ? PawnOwner->GetWorldSettings() : NULL;
	if (!WorldSettings || !UpdatedComponent)
	{
		return false;
	}

	const FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	if (CurrentLocation.Z < WorldSettings->KillZ)
	{
		Velocity.Z = FMath::Min(GetMaxSpeed(), WorldSettings->KillZ - CurrentLocation.Z + 2.0f);
		return true;
	}

	return false;
}

void UTsPhysicsCharacterMovement::InitializeCapsuleBasedMovement(UCapsuleComponent* CapsuleComponent, USkeletalMeshComponent* SkeletalMeshComp)
{
	RootDriveComponent = Cast<UShapeComponent>(CapsuleComponent);
	InitDriveType = 1;

	if (SkeletalMeshComp != nullptr)
	{
		CurrentSkeletalMeshComponent = SkeletalMeshComp;
	}
}

FRotator UTsPhysicsCharacterMovement::GetPhysicsRotation()
{
	return FRotator();
}

void UTsPhysicsCharacterMovement::ApplyControlInputToVelocity(float DeltaTime)
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);
	const float MaxPawnSpeed = GetMaxSpeed() * AnalogInputModifier;
	const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

	if (AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if (Velocity.SizeSquared() > 0.f)
		{
			// Change direction faster than only using acceleration, but never increase velocity magnitude.
			const float TimeScale = FMath::Clamp(DeltaTime * TurningBoost, 0.f, 1.f);
			Velocity = Velocity + (ControlAcceleration * Velocity.Size() - Velocity) * TimeScale;
		}
	}
	else
	{
		// Dampen velocity magnitude based on deceleration.
		if (Velocity.SizeSquared() > 0.f)
		{
			const FVector OldVelocity = Velocity;
			const float VelSize = FMath::Max(Velocity.Size() - FMath::Abs(Deceleration) * DeltaTime, 0.f);
			Velocity = Velocity.GetSafeNormal() * VelSize;

			// Don't allow braking to lower us below max speed if we started above it.
			if (bExceedingMaxSpeed && Velocity.SizeSquared() < FMath::Square(MaxPawnSpeed))
			{
				Velocity = OldVelocity.GetSafeNormal() * MaxPawnSpeed;
			}

			Velocity = FVector(0, 0, Velocity.Z);
		}
	}

	// floating capsule
	if (RootDriveComponent)
	{
		float SpringVelZ = 0;
		float x = 0;
		FCollisionQueryParams TraceParams(FName(TEXT("PhysicsMovementTrace")), true, NULL);

		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult HitDetails = FHitResult(ForceInit);

		FVector StartPosition = RootDriveComponent->GetRelativeLocation();

		FVector DownwardDirection = RootDriveComponent->GetUpVector() * -1;

		DownwardDirection.Normalize();

		bool bIsHit = GetWorld()->LineTraceSingleByChannel(
			HitDetails,
			StartPosition,
			DownwardDirection * 1000 + StartPosition,
			ECC_PhysicsBody,
			TraceParams
		);


		if (bIsHit)
		{
			FString Msg = "Hit Object: " + (HitDetails.GetComponent()->GetName() + " Distance: " + FString::SanitizeFloat(HitDetails.Distance)) + " DELTATIME: " + FString::SanitizeFloat(DeltaTime);
			print(Msg);
			x = HitDetails.Distance - RideHeight;
		}

		//float SpringForce = -(x * RideSpringStrength) + GetGravityZ() / 100 * RootDriveComponent->GetMass();

		float SpringForce = -(x * RideSpringStrength) - RideSpringDamper * Velocity.Z;

		float Acceln = SpringForce / RootDriveComponent->GetMass();

		float DeltaVel = Acceln * DeltaTime;


		// must use its physics Vertical velocity(For gravity reason)
		Velocity = FVector(Velocity.X, Velocity.Y, Velocity.Z + DeltaVel);
	}

	// Apply acceleration and clamp velocity magnitude.
	const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Size() : MaxPawnSpeed;
	Velocity += ControlAcceleration * FMath::Abs(Acceleration) * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(NewMaxSpeed);

	ConsumeInputVector();
}

void UTsPhysicsCharacterMovement::PhysicsTick_Implementation(float SubstepDeltaTime)
{
	// physics calculation here..
	const AController* Controller = PawnOwner->GetController();

	//SubstepDeltaTime = 0.016666f;


	if (Controller && Controller->IsLocalController())
	{
		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() == true || Controller->IsFollowingAPath() == false || bUseAccelerationForPaths)
		{
			ApplyControlInputToVelocity(SubstepDeltaTime);
		}
		// if it's not player controller, but we do have a controller, then it's AI
		// (that's not following a path) and we need to limit the speed
		else if (IsExceedingMaxSpeed(MaxSpeed) == true)
		{
			Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
		}

		LimitWorldBounds();
		bPositionCorrected = false;

		// Move actor
		FVector Delta = Velocity * SubstepDeltaTime;


		if (!Delta.IsNearlyZero(1e-6f))
		{
			const FVector OldLocation = RootDriveComponent->GetComponentLocation();
			const FQuat Rotation = RootDriveComponent->GetComponentQuat();

			FHitResult Hit(1.f);
			SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

			if (Hit.IsValidBlockingHit())
			{
				HandleImpact(Hit, SubstepDeltaTime, Delta);
				// Try to slide the remaining distance along the surface.
				SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
			}

			// Update velocity
			// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
			if (!bPositionCorrected)
			{
				const FVector NewLocation = RootDriveComponent->GetComponentLocation();
				Velocity = ((NewLocation - OldLocation) / SubstepDeltaTime);
			}
		}


		// Final Update of Root Drive and Visual Update.
		if (RootDriveComponent != nullptr)
		{
			if (CurrentSkeletalMeshComponent != nullptr)
			{
				CurrentSkeletalMeshComponent->SetAllPhysicsPosition(RootDriveComponent->GetRelativeLocation() + FVector(0, 0, MeshHeightOffset));
				CurrentSkeletalMeshComponent->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
			}

			RootDriveComponent->SetPhysicsLinearVelocity(Velocity, false);

		}
	}


}

void UTsPhysicsCharacterMovement::CustomPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{

	int cur_Substep = FMath::Clamp<int>(1, 8, FMath::CeilToInt(GetWorld()->GetDeltaSeconds() * 60));
	PhysicsTick(DeltaTime / cur_Substep);
}

bool UTsPhysicsCharacterMovement::ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotationQuat)
{
	bPositionCorrected |= Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotationQuat);
	return bPositionCorrected;
}