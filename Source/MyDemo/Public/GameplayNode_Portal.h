// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayNodeBase.h"
#include "GameplayNode_Portal.generated.h"

/**
 * 
 */
UCLASS()
class MYDEMO_API AGameplayNode_Portal : public AGamePlayNodeBase
{
	GENERATED_BODY()

public:
	AGameplayNode_Portal();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Components" )
	class UStaticMeshComponent* SecondMeshComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Components")
	class USphereComponent* SecondCheckComp;
};
