// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyDemoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
//#include "AnimGraphRuntime/Public/AnimNodes/AnimNode_RandomPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyDemo/Public/IndicatorLine.h"
#include "MyComponent/ACBuildSystem.h"
#include "Public/GamePlayNodeBase.h"
#include "Public/GamePlayNodesCheckComp.h"

//////////////////////////////////////////////////////////////////////////
// AMyDemoCharacter

AMyDemoCharacter::AMyDemoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

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

	GameplayNodeCheckComp = CreateDefaultSubobject<UGamePlayNodesCheckComp>(TEXT("GameplayNodeCheckComp"));
	GameplayNodeCheckComp->TraceRadius = 50.f;
	GameplayNodeCheckComp->TraceDistance = 2000.f;

	InitRotator = GetActorRotation();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyDemoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	BindMyBuildSystemAction(PlayerInputComponent); 
	BindMySystemAction(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyDemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyDemoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyDemoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyDemoCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyDemoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyDemoCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyDemoCharacter::OnResetVR);
}

FVector AMyDemoCharacter::GetLineTraceOrigin()
{

	FVector CamLoc;
	FRotator CamRot;
	GetController()->GetPlayerViewPoint(CamLoc, CamRot);
	return CamLoc;
}

FRotator AMyDemoCharacter::GetLineTraceRotator()
{

	FVector CamLoc;
	FRotator CamRot;
	GetController()->GetPlayerViewPoint(CamLoc, CamRot);
	return CamRot;
}


void AMyDemoCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Spawn origin indicator line 
	FActorSpawnParameters ASP;
	ASP.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	OriginIndicatorLine = GetWorld()->SpawnActor<AIndicatorLine>(IndicatorLineClass,GetActorLocation(),GetActorRotation(),ASP);
	if(OriginIndicatorLine)
	{
		OriginIndicatorLine->AttachToActor(this,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		OriginIndicatorLine->StartPoint = GetActorLocation();
		OriginIndicatorLine->EndPoint =	OriginIndicatorLine->StartPoint + UKismetMathLibrary::GetForwardVector(GetActorRotation()) * OriginIndicatorLine->DefaultLength;
		OriginIndicatorLine->StartObject = MakeWeakObjectPtr(this);
	}
	//初始化第一个节点
	if (GameplayNodeCheckComp->FirstNode)
	{
		TargetNode = GameplayNodeCheckComp->FirstNode;
	}
}

void AMyDemoCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bCanMoving && TargetNode)
	{
		FVector InterpLocation  = 	UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(),TargetNode->OutLocation,DeltaSeconds,MoveSpeed);
		SetActorLocation(InterpLocation);
		if(GetActorLocation().Equals(TargetNode->OutLocation,5.0f))
		{
			CurrentNode = TargetNode;
			CurrentNode->NodeExecutionDelegate.Broadcast(this,CurrentNode->NodeTag);
			TargetNode = CurrentNode->GetNextNode();
		}
	}
}

void AMyDemoCharacter::InitFirstNode()
{
	if (GameplayNodeCheckComp->FirstNode)
	{
		TargetNode = GameplayNodeCheckComp->FirstNode;
	}
}

void AMyDemoCharacter::OnResetVR()
{
	// If MyDemo is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in MyDemo.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyDemoCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMyDemoCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMyDemoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyDemoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyDemoCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();

		//Rotation = GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyDemoCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		FRotator Rotation = Controller->GetControlRotation();
		//Rotation = GetActorRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMyDemoCharacter::PickUp(FPickupInfo i) {
	FBuildableInfo binfo;
	binfo.actor_class = i.actor_class;
	binfo.count = i.count;
	binfo.name = i.name;
	binfo.SMGhost = i.SMGhost;
	binfo.Icon = i.Icon;
	binfo.GhostScale = i.GhostScale;
	AddBuildableInfo(binfo);
}