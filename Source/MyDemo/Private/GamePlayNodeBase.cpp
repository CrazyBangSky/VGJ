// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDemo/Public/GamePlayNodeBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGamePlayNodeBase::AGamePlayNodeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CheckComp = CreateDefaultSubobject<USphereComponent>(TEXT("CheckComp"));
	CheckComp->SetSphereRadius(CheckRadius);
	CheckComp->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AGamePlayNodeBase::BeginPlay()
{
	Super::BeginPlay();
	NodeExecutionDelegate.AddDynamic(this,&AGamePlayNodeBase::OnNodeExecution);
}

// Called every frame
void AGamePlayNodeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGamePlayNodeBase::ActiveNode_Implementation(AActor* _Executant, FName _NodeTag)
{
}

void AGamePlayNodeBase::UpdateBuildState(bool bBuilding)
{
	bHasBeenBuilt = bBuilding;
	if (bHasBeenBuilt)
	{
		MeshComp->SetMaterial(0,NormalMaterial);
	}
	else
	{
		MeshComp->SetMaterial(0,GhostMaterial);
	}
}

void AGamePlayNodeBase::OnNodeExecution(AActor* _Executant, FName _NodeTag)
{
	if(_Executant)
	{
		const FVector DefaultUpVector = FVector(0.f,0.f,1.f);
		ActiveNode(_Executant,NodeTag);
		//_Executant->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(OutDirectiron,DefaultUpVector));
	}
}

