// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyDemo/MyComponent/ACBuildSystem.h"
#include "VGJGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYDEMO_API AVGJGameMode : public AGameMode
{
	GENERATED_BODY()
	AVGJGameMode();

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBuildableInfo>	CUrrentLevelBuildInfos;
	
};
