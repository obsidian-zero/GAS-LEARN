// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/HitTraceComponent/HitTraceComponent.h"

#include "Kismet/BlueprintTypeConversions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values for this component's properties
UHitTraceComponent::UHitTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; // 允许 Tick
	
}


// Called when the game starts
void UHitTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false); // 禁用 Tick
	
}


// Called every frame
void UHitTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto& ActiveHitTraceTask : ActiveHitTraceTaskMap) // 注意这里的 auto& 
	{
		FHitTraceRunTime& HitTraceRunTime = ActiveHitTraceTask.Value; // 使用引用
		if (HitTraceRunTime.checkTime >= HitTraceRunTime.HitTraceTaskInfo.Interval)
		{
			HitTrace(HitTraceRunTime);
		}
		else
		{
			HitTraceRunTime.checkTime += DeltaTime;
		}
	}
}


void UHitTraceComponent::InitHitTraceInfo(FHitTraceTaskInfo HitTraceTaskInfo)
{
	if(HitTraceTaskMap.Contains(HitTraceTaskInfo.TaskName))
	{
		UE_LOG(LogTemp, Warning, TEXT("HitTraceTaskMap already contains %s"), *HitTraceTaskInfo.TaskName);
	}
	else
	{
		HitTraceTaskMap.Add(HitTraceTaskInfo.TaskName, FHitTraceRunTime(HitTraceTaskInfo));
	}
}

void UHitTraceComponent::StartHitTraceByName(FString HitTraceTaskName)
{
	if(HitTraceTaskMap.Contains(HitTraceTaskName)){
		if(!ActiveHitTraceTaskMap.Contains(HitTraceTaskName))
		{
			
			ActiveHitTraceTaskMap.Add(HitTraceTaskName, HitTraceTaskMap[HitTraceTaskName]);
			ActivateHitTrace(HitTraceTaskMap[HitTraceTaskName]);
			if(ActiveHitTraceTaskMap.Num() > 0)
			{
				SetComponentTickEnabled(true);
			}
		}
	}
}

void UHitTraceComponent::EndHitTraceByName(FString HitTraceTaskName)
{
	if(ActiveHitTraceTaskMap.Contains(HitTraceTaskName))
	{
		ActiveHitTraceTaskMap.Remove(HitTraceTaskName);
		if(ActiveHitTraceTaskMap.Num() == 0)
		{
			SetComponentTickEnabled(false);
		}
	}
}

void UHitTraceComponent::HitTrace(FHitTraceRunTime& HitTraceRunTime)
{
	FVector startPos = HitTraceRunTime.HitTraceTaskInfo.Mesh->GetSocketLocation(HitTraceRunTime.HitTraceTaskInfo.BeginSocket);
	FVector endPos = HitTraceRunTime.HitTraceTaskInfo.Mesh->GetSocketLocation(HitTraceRunTime.HitTraceTaskInfo.EndSocket);
	
	// 计算盒子的中心位置
	FVector Start = (startPos + HitTraceRunTime.lastStartPos) / 2;
	FVector End = (endPos + HitTraceRunTime.lastEndPos) / 2;
	
	FVector BoxExtent = FVector(HitTraceRunTime.HitTraceTaskInfo.Radius, FVector::Distance(HitTraceRunTime.lastEndPos, endPos) / 2, HitTraceRunTime.HitTraceTaskInfo.Radius); // 50 是 X 方向，10 是 Y 和 Z 方向的大小

	// 碰撞通道（例如：ECC_WorldDynamic）
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // 添加 Pawn 对象类型

	TArray<FHitResult> HitResults;

	FRotator que = UKismetMathLibrary::FindLookAtRotation(Start, End); // 旋转
	// 执行盒子检测
	bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetWorld(),
		Start, // 使用中心位置
		End, // 结束位置
		BoxExtent,
		que, // 旋转
		HitTraceRunTime.HitTraceTaskInfo.ObjectTypes,
		false, // 是否进行复杂检测
		HitTraceRunTime.IgnoreActors, // 忽略的 Actor 列表
		HitTraceRunTime.HitTraceTaskInfo.DrawDebugType, // 调试可视化选项
		HitResults,
		HitTraceRunTime.HitTraceTaskInfo.IgnoreSelf,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
	);
	if(bHit)
	{
		for(FHitResult& HitResult : HitResults)
		{
			if(AActor * HitActor = HitResult.GetActor())
			{
				HitTraceRunTime.HitActors.Add(HitActor);
				HitTraceRunTime.IgnoreActors.Add(HitActor);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit Actor: %s"), *HitActor->GetName()));
			}
		}
	}

	HitTraceRunTime.lastStartPos = startPos;
	HitTraceRunTime.lastEndPos = endPos;
	HitTraceRunTime.checkTime = 0;
}


void UHitTraceComponent::ActivateHitTrace_Implementation(FHitTraceRunTime& HitTraceRunTime)
{
	FVector startPos = HitTraceRunTime.HitTraceTaskInfo.Mesh->GetSocketLocation(HitTraceRunTime.HitTraceTaskInfo.BeginSocket);
	FVector endPos = HitTraceRunTime.HitTraceTaskInfo.Mesh->GetSocketLocation(HitTraceRunTime.HitTraceTaskInfo.EndSocket);
	
	HitTraceRunTime.lastStartPos = startPos;
	HitTraceRunTime.lastEndPos = endPos;
	HitTraceRunTime.checkTime = 0;
	HitTraceRunTime.IgnoreActors = HitTraceRunTime.HitTraceTaskInfo.IgnoreActors;
	HitTraceRunTime.HitActors.Reset();
}
