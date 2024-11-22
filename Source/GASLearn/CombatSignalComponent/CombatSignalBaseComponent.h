// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/NetDriver.h"  
#include "Engine/EngineBaseTypes.h"
#include "Engine/DataTable.h"
#include "CombatSignalBaseComponent.generated.h"

USTRUCT(BlueprintType)
struct FCombatSignalDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 signalID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SelfText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FriendText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SelfSound = nullptr;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FriendSound = nullptr;  
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASLEARN_API UCombatSignalBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatSignalBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 客户端请求发送战斗信号的服务器RPC, 不必非要保证可靠性
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void SendCombatNotification(int32 NotificationID);
	void SendCombatNotification_Implementation(int32 NotificationID);

	// 多播RPC，用于通知战斗信号给所有客户端
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable)
	void NotifyCombatNotification(int32 NotificationID);
	void NotifyCombatNotification_Implementation(int32 NotificationID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* SignalDataTable;
};
