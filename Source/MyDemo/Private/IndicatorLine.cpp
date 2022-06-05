// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDemo//Public/IndicatorLine.h"

#include "DrawDebugHelpers.h"

// Sets default values
AIndicatorLine::AIndicatorLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIndicatorLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIndicatorLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDrawDebugLine)
	{
		DrawDebugSphere(GetWorld(),StartPoint,50.0f,20.0f,FColor::Red,false,0.0f);
		DrawDebugSphere(GetWorld(),EndPoint,50.0f,20.0f,FColor::Green,false,0.0f);
		DrawDebugLine(GetWorld(),StartPoint,EndPoint,FColor::Cyan,false,0.0f);
	}
	
}

