// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyComponent/IMyBuildSystemBuilder.h"
#include "MyComponent/PickupComponent/IPicker.h" 
#include "Public/IndicatorLine.h"
#include "MyDemoCharacter.generated.h"

UCLASS(config=Game)
class AMyDemoCharacter : public ACharacter, public IMyBuildSystemBuilder, public IPicker
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMyDemoCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UGamePlayNodesCheckComp* GameplayNodeCheckComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AIndicatorLine> IndicatorLineClass; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AIndicatorLine* OriginIndicatorLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CurrentDirection;

	UPROPERTY(BlueprintReadWrite)
	FRotator InitRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotateSpeed;

	UPROPERTY(BlueprintReadWrite)
	class AGamePlayNodeBase* CurrentNode;

	UPROPERTY(BlueprintReadWrite)
	class AGamePlayNodeBase* TargetNode;

	UPROPERTY(BlueprintReadWrite)
	bool bCanMoving;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void InitFirstNode();

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


public:

	virtual FVector GetLineTraceOrigin() override;
	virtual FRotator GetLineTraceRotator() override;

	// picker 
	virtual void PickUp(FPickupInfo i) override;
};

