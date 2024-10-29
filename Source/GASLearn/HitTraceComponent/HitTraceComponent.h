// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitTraceComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHitTraceDelegate, const FHitResult&, HitResult, FString, TaskName, AActor* ,HitActor);

USTRUCT(BlueprintType)
struct FHitTraceTaskInfo
{
	GENERATED_BODY()
	
	/** 命中检测的任务名称 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="TaskName", MakeStructureDefaultValue="None"))
	FString TaskName;
	
	/** 攻击使用Mesh信息 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Mesh", MakeStructureDefaultValue="None"))
	TObjectPtr<USceneComponent> Mesh;

	/** 攻击检测起始插槽 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="BeginSocket", MakeStructureDefaultValue="None"))
	FName BeginSocket;

	/** 攻击检测结束插槽 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="EndSocket", MakeStructureDefaultValue="None"))
	FName EndSocket;

	/** 攻击检测忽略的Actor */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta=(DisplayName="IgnoreActors"))
	TArray<AActor*> IgnoreActors;

	/** 攻击检测的Actor类型 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="ObjectTypes"))
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	
	/** 攻击检测的范围 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Radius", MakeStructureDefaultValue="0.000000"))
	double Radius = 0.0f;

	/** 攻击检测的间隔 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Interval", MakeStructureDefaultValue="0.000000"))
	double Interval = 1.0f;
	
	/** 攻击检测的Debug信息类型 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="DrawDebugType", MakeStructureDefaultValue="None"))
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	/** 攻击debug显示市场*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="DrawDebugTime", MakeStructureDefaultValue="0.000000"))
	double DrawDebugTime = 0.0f;
	
	/** 攻击检测是否忽略自身 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="IgnoreSelf", MakeStructureDefaultValue="False"))
	bool IgnoreSelf = true;
	
	/** 攻击检测的来源 */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta=(DisplayName="Instigator", MakeStructureDefaultValue="None"))
	TObjectPtr<AActor> Instigator;
};

USTRUCT(BlueprintType)
struct FHitTraceRunTime
{
	GENERATED_BODY()
	
	FHitTraceTaskInfo HitTraceTaskInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor *> HitActors = TArray<AActor*>();

	float checkTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor *> IgnoreActors = TArray<AActor*>();

	FVector lastStartPos = FVector::ZeroVector;
	FVector lastEndPos = FVector::ZeroVector;

	// 默认构造函数
	FHitTraceRunTime()
		:	HitTraceTaskInfo(), // 调用默认构造函数
			HitActors(),        // 初始化 TArray
			checkTime(0.0f),    // 初始化 checkTime
			IgnoreActors(),		// 初始化 TArray
			lastStartPos(FVector::ZeroVector), // 初始化 lastStartPos
			lastEndPos(FVector::ZeroVector)    // 初始化 lastEndPos
		{
		}
	
	// 带参数的构造函数
	FHitTraceRunTime(FHitTraceTaskInfo InHitTraceTaskInfo)
		:	HitTraceTaskInfo(InHitTraceTaskInfo), // 使用成员初始化列表
			HitActors(),        // 初始化 TArray
			checkTime(0.0f),    // 初始化 checkTime
			IgnoreActors(),		// 初始化 TArray
			lastStartPos(FVector::ZeroVector), // 初始化 lastStartPos
			lastEndPos(FVector::ZeroVector)    // 初始化 lastEndPos
		{
		}
};

UCLASS(Blueprintable,  ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASLEARN_API UHitTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** 命中追踪的信息列表 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="HitTraceTaskMap", MakeStructureDefaultValue="()"))
	TMap<FString,FHitTraceRunTime> HitTraceTaskMap;

	/** 活跃的命中追踪的信息列表 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="HitTraceTaskMap", MakeStructureDefaultValue="()"))
	TMap<FString,FHitTraceRunTime> ActiveHitTraceTaskMap;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category="HitTraceComponent")
	void InitHitTraceInfo(FHitTraceTaskInfo HitTraceTaskInfo);

	UFUNCTION(BlueprintCallable, Category="HitTraceComponent")
	void StartHitTraceByName(FString HitTraceTaskName);
	
	UFUNCTION(BlueprintCallable, Category="HitTraceComponent")
	void EndHitTraceByName(FString HitTraceTaskName);

	UFUNCTION(BlueprintCallable, Category="HitTraceComponent")
	void HitTrace(FHitTraceRunTime& HitTraceRunTime);

	UFUNCTION(BlueprintNativeEvent, Category="HitTraceComponent")
	void ActivateHitTrace(FHitTraceRunTime& HitTraceRunTime);

	UPROPERTY(BlueprintAssignable)
	FOnHitTraceDelegate OnHitDetected;
};
