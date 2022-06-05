// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamePlayNodeBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNodeExecution, AActor*,Executant ,FName, NodeTag);

UCLASS()
class MYDEMO_API AGamePlayNodeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamePlayNodeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* CheckComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float CheckRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FName NodeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OutDirectiron;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OutLocation;

	UPROPERTY(BlueprintAssignable)
	FOnNodeExecution NodeExecutionDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* GhostMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* NormalMaterial;

	UPROPERTY(BlueprintReadWrite)
	bool bHasBeenBuilt;

	UFUNCTION(BlueprintNativeEvent)
	void ActiveNode();
	virtual  void ActiveNode_Implementation();
	
	UFUNCTION(BlueprintPure)
	AGamePlayNodeBase* GetNextNode() const { return  NextNode.Get();};

	UFUNCTION(BlueprintCallable)
	virtual void UpdateBuildState(bool bBuilding);
	
	TWeakObjectPtr<AGamePlayNodeBase> NextNode;

	UFUNCTION()
	void OnNodeExecution(AActor* _Executant ,FName _NodeTag);

};
