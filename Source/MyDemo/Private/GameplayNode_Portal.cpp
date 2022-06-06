// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayNode_Portal.h"

#include "Components/SphereComponent.h"

AGameplayNode_Portal::AGameplayNode_Portal()
{
	NodeTag = FName(TEXT("Portal"));

	SecondMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondMeshComp"));
	SecondMeshComp->SetupAttachment(RootComp);
	SecondMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SecondCheckComp = CreateDefaultSubobject<USphereComponent>(TEXT("SecondCheckComp"));
	SecondCheckComp->SetupAttachment(SecondMeshComp);
}
