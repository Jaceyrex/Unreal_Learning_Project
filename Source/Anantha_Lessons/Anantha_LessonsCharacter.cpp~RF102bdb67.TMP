// Copyright Epic Games, Inc. All Rights Reserved.

#include "Anantha_LessonsCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Misc/App.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/ArrowComponent.h"


//////////////////////////////////////////////////////////////////////////
// AAnantha_LessonsCharacter

AAnantha_LessonsCharacter::AAnantha_LessonsCharacter()
{
	//Replication
	bReplicates = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//Sets sprint Modifier value
	sprintModifier = 4;

	//Sets current charge to 0 for shooting
	currentCharge = 0;

	//Enum to switch sides shots will spawn from
	side = Side::Right;

	//Side since it wants to be awkward :')
	intSide = 1;

	////gets anchors from skeleton sockets
	//LeftAnchor = this->GetMesh()->GetSocketByName("Left Shot Anchor");
	//RightAnchor = this->GetMesh()->GetSocketByName("Right Shot Anchor");

	LeftAnchor = CreateDefaultSubobject<UArrowComponent>(TEXT("Left_Anchor"));
	LeftAnchor->SetupAttachment(GetFollowCamera());
	RightAnchor = CreateDefaultSubobject<UArrowComponent>(TEXT("Right_Anchor"));
	RightAnchor->SetupAttachment(GetFollowCamera());

	SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAnantha_LessonsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAnantha_LessonsCharacter::StartSprint_Implementation);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AAnantha_LessonsCharacter::EndSprint_Implementation);

	PlayerInputComponent->BindAction("RangedAttack", IE_Pressed, this, &AAnantha_LessonsCharacter::StartShooting);
	PlayerInputComponent->BindAction("RangedAttack", IE_Repeat, this, &AAnantha_LessonsCharacter::ChargeShot);
	PlayerInputComponent->BindAction("RangedAttack", IE_Pressed, this, &AAnantha_LessonsCharacter::EndShooting);


PlayerInputComponent->BindAxis("MoveForward", this, &AAnantha_LessonsCharacter::MoveForward);
PlayerInputComponent->BindAxis("MoveRight", this, &AAnantha_LessonsCharacter::MoveRight);

// We have 2 versions of the rotation bindings to handle different kinds of devices differently
// "turn" handles devices that provide an absolute delta, such as a mouse.
// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
PlayerInputComponent->BindAxis("TurnRate", this, &AAnantha_LessonsCharacter::TurnAtRate);
PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
PlayerInputComponent->BindAxis("LookUpRate", this, &AAnantha_LessonsCharacter::LookUpAtRate);

// handle touch devices
PlayerInputComponent->BindTouch(IE_Pressed, this, &AAnantha_LessonsCharacter::TouchStarted);
PlayerInputComponent->BindTouch(IE_Released, this, &AAnantha_LessonsCharacter::TouchStopped);

// VR headset functionality
PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAnantha_LessonsCharacter::OnResetVR);
}


void AAnantha_LessonsCharacter::OnResetVR()
{
	// If Anantha_Lessons is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in Anantha_Lessons.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAnantha_LessonsCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AAnantha_LessonsCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AAnantha_LessonsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAnantha_LessonsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAnantha_LessonsCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAnantha_LessonsCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

UFUNCTION(NetMulticast, Reliable)
void AAnantha_LessonsCharacter::StartSprint_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Start sprint"));
	GetCharacterMovement()->MaxWalkSpeed *= sprintModifier;
}

UFUNCTION(NetMulticast, Reliable)
void AAnantha_LessonsCharacter::EndSprint_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("End sprint"));
	GetCharacterMovement()->MaxWalkSpeed /= sprintModifier;
}

void AAnantha_LessonsCharacter::StartShooting()
{
	UE_LOG(LogTemp, Warning, TEXT("shoot button down"));


	FVector LeftLocation;
	FRotator LeftRotation;
	FVector RightLocation;
	FRotator RightRotation;

	if (LeftAnchor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Left anchor not null"));

		LeftLocation = LeftAnchor->GetComponentLocation();
		//LeftLocation = LeftAnchor->GetWorldLocation();
		LeftRotation = LeftAnchor->GetComponentRotation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("right anchor IS NULL"));
	}

	if (RightAnchor)
	{
		UE_LOG(LogTemp, Warning, TEXT("right anchor not null"));
		RightLocation = RightAnchor->GetComponentLocation();
		RightRotation = RightAnchor->GetComponentRotation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("right anchor IS NULL"));
	}



	FActorSpawnParameters SpawnInfo;
	//switch (side)
	//{
	//	case Side::Left: 
	//		UE_LOG(LogTemp, Warning, TEXT("About to fire from left"));

	//		/*this->Children[0]->Children[2]->Children[0]->Children[0]
	//		
	//		this->Children
	//		this->GetComponents()*/
	//		//GetWorld()->SpawnActor<AActor>()
	//		GetWorld()->SpawnActor<AActor>(ToSpawn, LeftLocation, LeftRotation, SpawnInfo);
	//		//Get the current shot
	//		UE_LOG(LogTemp, Warning, TEXT("Fired from left"));

	//		side = Side::Right;
	//		UE_LOG(LogTemp, Warning, TEXT("side variable set to RIGHT"));
	//		break;
	//	case Side::Right: 
	//		UE_LOG(LogTemp, Warning, TEXT("About to fire from right"));

	//		GetWorld()->SpawnActor<AActor>(ToSpawn, RightLocation, RightRotation, SpawnInfo);
	//		//GetWorld()->SpawnActor<AActor>(ToSpawn, FTransform(LeftLocation, LeftRotation, FVector(0.f)), SpawnInfo);
	//		//Get the current shot
	//		UE_LOG(LogTemp, Warning, TEXT("Fired from right"));

	//		side = Side::Left;
	//		UE_LOG(LogTemp, Warning, TEXT("side variable set to RIGHT"));
	//		break;
	//}

	switch (intSide)
	{
	case 1:
		UE_LOG(LogTemp, Warning, TEXT("About to fire from left"));

		/*this->Children[0]->Children[2]->Children[0]->Children[0]

		this->Children
		this->GetComponents()*/
		//GetWorld()->SpawnActor<AActor>()
		GetWorld()->SpawnActor<AActor>(ToSpawn, LeftLocation, LeftRotation, SpawnInfo);
		//Get the current shot
		UE_LOG(LogTemp, Warning, TEXT("Fired from left"));

		intSide = 2;
		UE_LOG(LogTemp, Warning, TEXT("side variable set to RIGHT"));
		break;
	case 2:
		UE_LOG(LogTemp, Warning, TEXT("About to fire from right"));

		GetWorld()->SpawnActor<AActor>(ToSpawn, RightLocation, RightRotation, SpawnInfo);
		//GetWorld()->SpawnActor<AActor>(ToSpawn, FTransform(LeftLocation, LeftRotation, FVector(0.f)), SpawnInfo);
		//Get the current shot
		UE_LOG(LogTemp, Warning, TEXT("Fired from right"));

		intSide = 1;
		UE_LOG(LogTemp, Warning, TEXT("side variable set to RIGHT"));
		break;
	}
}

void AAnantha_LessonsCharacter::ChargeShot()
{
	currentCharge += chargeRate; //* DeltaTime - can't get it to work :(
	//CurrentShot Scale = currentcharge

}

void AAnantha_LessonsCharacter::EndShooting()
{

	if (side == Side::Left)
	{
		side = Side::Right;
	}
	else if (side == Side::Right)
	{
		side = Side::Left;
	}
	currentCharge = 0;
}

float AAnantha_LessonsCharacter::GetCurrentCharge()
{
	return currentCharge;
}
