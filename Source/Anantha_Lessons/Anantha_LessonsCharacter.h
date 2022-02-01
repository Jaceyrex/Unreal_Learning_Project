// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Anantha_LessonsCharacter.generated.h"

UENUM(BlueprintType)
enum class Side : uint8
{
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
};

UCLASS(config=Game)
class AAnantha_LessonsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AAnantha_LessonsCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Movement)
	float sprintModifier;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shooting)
	float currentCharge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shooting)
	float maxShotCharge;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shooting)
	float chargeRate;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shooting)
	//enum class Side;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shooting)
	Side side;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shooting)
	int intSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class AActor> ToSpawn;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	////TSubclassOf<class AActor> LeftAnchor;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//TSubclassOf<class AActor> RightAnchor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class UArrowComponent* LeftAnchor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class UArrowComponent* RightAnchor;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	//const USkeletalMeshSocket* LeftAnchor;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	//const USkeletalMeshSocket* RightAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USkeletalMeshComponent* SkeletalMesh;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);



protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//UFUNCTION(BlueprintCallable, Category = Movement)

	UFUNCTION(Server, Reliable)
	void StartSprint();

	UFUNCTION(Server, Reliable)
	void EndSprint();

	void StartShooting();
	void ChargeShot();
	void EndShooting();

	float GetCurrentCharge();
};

