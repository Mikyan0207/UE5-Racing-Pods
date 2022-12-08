// Fill out your copyright notice in the Description page of Project Settings.


#include "WheeledVehicle.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AWheeledVehicle::AWheeledVehicle()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AWheeledVehicle::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void AWheeledVehicle::Throttle(const FInputActionValue& Value)
{
	float throttle = Value.Get<float>();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, *FString::SanitizeFloat(throttle));

	GetVehicleMovementComponent()->SetThrottleInput(throttle);
}

void AWheeledVehicle::Steering(const FInputActionValue& Value)
{
	float steering = Value.Get<float>();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, *FString::SanitizeFloat(steering));

	GetVehicleMovementComponent()->SetSteeringInput(steering);
}

void AWheeledVehicle::HandbrakePressed(const FInputActionValue& Value)
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AWheeledVehicle::HandbrakeReleased(const FInputActionValue& Value)
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

/** Called for looking input */
void AWheeledVehicle::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWheeledVehicle::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AWheeledVehicle::SetupPlayerInputComponent(class UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(playerInputComponent)) {

		//Moving
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AWheeledVehicle::Throttle);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AWheeledVehicle::Steering);

		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &AWheeledVehicle::HandbrakePressed);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AWheeledVehicle::HandbrakeReleased);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWheeledVehicle::Look);
	}
}