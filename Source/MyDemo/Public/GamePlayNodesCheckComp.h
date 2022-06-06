// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePlayNodesCheckComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYDEMO_API UGamePlayNodesCheckComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGamePlayNodesCheckComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float TraceRadius;
	
	TMap<class AGamePlayNodeBase*,int32> CheckNodesCounterMap;

	UPROPERTY(BlueprintReadWrite)
	class AGamePlayNodeBase* FirstNode = nullptr;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxCheckCount = 10;
	
private:
	AActor* MyOwner;

	FCollisionQueryParams TraceParams;
	FCollisionObjectQueryParams CheckQueryParams;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypoeQueryList;
	
	bool TraceCheck(FVector Start,FVector Direction, class AGamePlayNodeBase* TraceNode);
	
	void UpdateAllNodesOutDirection(class AGamePlayNodeBase* Node);
};
