// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "WheeledVehiclePawn.h"
#include "WheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class RACINGPODS_API AWheeledVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

	AWheeledVehicle();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* AudioSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* EngineStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* Engine;

#pragma region Input Actions

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SteeringAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* HandbrakeAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

#pragma endregion

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Throttle(const FInputActionValue& Value);

	void Steering(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void HandbrakePressed(const FInputActionValue& Value);
	void HandbrakeReleased(const FInputActionValue& Value);

public:
	virtual void Tick(float deltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	bool m_IsEngineOn = false;

};
