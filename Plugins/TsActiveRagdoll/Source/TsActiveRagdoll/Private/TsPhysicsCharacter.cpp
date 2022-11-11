// This is a Floating Capsule Physics Movement Character
// Copyright @ Tiger Shan 2022

#include "TsPhysicsCharacter.h"
#include "TsPhysicsPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ATsPhysicsCharacter::ATsPhysicsCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);


	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->SetupAttachment(SceneComponent);

	CapsuleComponent->SetSimulatePhysics(true);

	// TODO: Current down manually through Blueprint.
	// we also need to set collision profile
	// we also need to lock certain axis for physics Simulation

	PhysicsCharacterMovement = CreateDefaultSubobject<UTsPhysicsCharacterMovement>(TEXT("Character Physics Movement"));

	PhysicsCharacterMovement->SetUpdatedComponent(CapsuleComponent);



	MoveScale = 1;
}

void ATsPhysicsCharacter::Move(const FInputActionValue& ActionValue)
{
	FVector Input = ActionValue.Get < FInputActionValue::Axis3D>();

	AddMovementInput(GetActorRotation().RotateVector(Input), MoveScale);


}

// Called when the game starts or when spawned
void ATsPhysicsCharacter::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ATsPhysicsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ATsPhysicsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ATsPhysicsPlayerController* PhysicsPC = Cast<ATsPhysicsPlayerController>(Controller);

	check(EIC && PhysicsPC);

	/**
	 * @brief  bind our actions
	 * @param PlayerInputComponent
	*/
	EIC->BindAction(PhysicsPC->MoveAction, ETriggerEvent::Triggered, this, &ATsPhysicsCharacter::Move);

	ULocalPlayer* LocalPlayer = PhysicsPC->GetLocalPlayer();

	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(EISubsystem);

	EISubsystem->ClearAllMappings();
	EISubsystem->AddMappingContext(PhysicsPC->PawnMappingContext, 0);


}

