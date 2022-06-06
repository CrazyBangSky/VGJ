// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDemo//Public/GamePlayNodesCheckComp.h"

#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyDemo/MyDemo.h"
#include "MyDemo/Public/GamePlayNodeBase.h"

// Sets default values for this component's properties
UGamePlayNodesCheckComp::UGamePlayNodesCheckComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ObjectTypoeQueryList.Add(UEngineTypes::ConvertToObjectType(TRACE_GAME_PLAY_NODE));
	// ...
}


// Called when the game starts
void UGamePlayNodesCheckComp::BeginPlay()
{
	Super::BeginPlay();
	MyOwner = GetOwner();
	TraceParams = FCollisionQueryParams(FName(TEXT("GamePlayNode_Check")), true, MyOwner);
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = true;
	//CheckQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	CheckQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	CheckQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	CheckQueryParams.AddObjectTypesToQuery(TRACE_GAME_PLAY_NODE);
	CheckQueryParams.AddObjectTypesToQuery(TRACE_GHOST);
	// ...
	
}


// Called every frame
void UGamePlayNodesCheckComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (MyOwner)
	{
		CheckNodesCounterMap.Empty();
		if(FirstNode)
		{
			AGamePlayNodeBase* CheckNode = FirstNode;
			while(CheckNode->NextNode.IsValid())
			{
				TWeakObjectPtr<AGamePlayNodeBase> Temp = CheckNode->NextNode;
				CheckNode->NextNode.Reset();
				CheckNode = Temp.Get();
			}
		}
		FHitResult HitResult;
		FVector End = MyOwner->GetActorLocation() + UKismetMathLibrary::GetForwardVector(MyOwner->GetActorRotation()) * TraceDistance;
		TArray<AActor*>IgnoreList = {};
		IgnoreList.Add(MyOwner);
		UKismetSystemLibrary::SphereTraceSingleForObjects(this,MyOwner->GetActorLocation(),End,TraceRadius,ObjectTypoeQueryList,false,IgnoreList,EDrawDebugTrace::ForOneFrame,HitResult,true);
		if (HitResult.bBlockingHit && Cast<AGamePlayNodeBase>(HitResult.Actor.Get()))
		{
			FirstNode = Cast<AGamePlayNodeBase>(HitResult.Actor.Get());
			TraceCheck(FirstNode->OutLocation,FirstNode->OutDirectiron,FirstNode);
		}
		UpdateAllNodesOutDirection(FirstNode);
	}
	// ...
}


bool UGamePlayNodesCheckComp::TraceCheck(FVector Start, FVector Direction,AGamePlayNodeBase* TraceNode)
{
	FHitResult HitResult;
	FVector End = Start + Direction * TraceDistance;
	TArray<AActor*>IgnoreList = {};
	IgnoreList.Add(TraceNode);
	UKismetSystemLibrary::SphereTraceSingleForObjects(this,Start,End,TraceRadius,ObjectTypoeQueryList,false,IgnoreList,EDrawDebugTrace::ForOneFrame,HitResult,true);
	if (!HitResult.bBlockingHit || Cast<AGamePlayNodeBase>(HitResult.Actor.Get()) == nullptr)
	{
		return  false;
	}
	//TODO:需要对传送门节点做特殊处理,必须检测到传送门入口才算检测成功
	AGamePlayNodeBase* Node = Cast<AGamePlayNodeBase>(HitResult.Actor.Get());
	if(CheckNodesCounterMap.Find(Node))
	{
		if(*(CheckNodesCounterMap.Find(Node)) == MaxCheckCount)
		{
			return  false;
		}
		else
		{
			CheckNodesCounterMap[Node] += 1;
		}
	}
	else
	{
		CheckNodesCounterMap.Add(TPair<AGamePlayNodeBase*,int32>(Node,1));
	}
	if(TraceNode)
	{
		TraceNode->NextNode = MakeWeakObjectPtr(Node);
	}
	Start = Node->OutLocation;
	Direction = Node->OutDirectiron;
	TraceNode = Node;
	return TraceCheck(Start,Direction,TraceNode);
}

void UGamePlayNodesCheckComp::UpdateAllNodesOutDirection(AGamePlayNodeBase* Node)
{
	if (Node)
	{
		if (Node->NextNode.IsValid())
		{
			Node->NextNode.Get()->OutDirectiron = UKismetMathLibrary::GetForwardVector(Node->GetActorRotation());
		}
		if (Node->NodeTag == FName(TEXT("Rotate")))
		{
			Node->OutDirectiron = UKismetMathLibrary::GetForwardVector(Node->GetActorRotation());
		}
		if (Node->NextNode.IsValid())
		{
			return UpdateAllNodesOutDirection(Node->NextNode.Get());
		}
	}
}

