// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/CombatSignalComponent/CombatSignalBaseComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UCombatSignalBaseComponent::UCombatSignalBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UCombatSignalBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatSignalBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatSignalBaseComponent::SendCombatNotification_Implementation(int32 NotificationID)
{
	// 服务器接收到客户端请求发送战斗信号的RPC后，调用多播RPC，同步战斗信号给所有客户端
	NotifyCombatNotification(NotificationID);
}

void UCombatSignalBaseComponent::NotifyCombatNotification_Implementation(int32 NotificationID)
{
	// 获取当前的网络角色和网络模式
	ENetRole LocalRole = GetOwner()->GetLocalRole();
	ENetMode NetMode = GetWorld()->GetNetMode();
	FString Name = GetOwner()->GetName();

	if(NetMode == NM_DedicatedServer)
	{
		return;
	}

	bool isSelf = LocalRole == ROLE_AutonomousProxy;
	
	if (!SignalDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SignalDataTable is not set."));
		return;
	}

	// 查找DataTable中的行
	static const FString ContextString(TEXT("GENERAL"));
	FCombatSignalDataTableRow* SignalData = SignalDataTable->FindRow<FCombatSignalDataTableRow>(FName(*FString::FromInt(NotificationID)), ContextString);

	if (!SignalData)
	{
		UE_LOG(LogTemp, Warning, TEXT("SignalData is not found."));
		return;
	}
	// 打印消息，包含信号ID、网络角色和网络模式
	if (GEngine)
	{
		FString Text = isSelf ? SignalData->SelfText : SignalData->FriendText;
		USoundBase * SoundBase = isSelf ? SignalData->SelfSound : SignalData->FriendSound;
		FColor MessageColor = isSelf ? FColor::Green : FColor::Blue;
		GEngine->AddOnScreenDebugMessage(
			-1, 5.f, MessageColor, 
			FString::Printf(TEXT("Loaded Signal Data: ID=%d, Text=%s"), NotificationID, *Text)
		);

		// 在指定位置播放声音，这里使用组件的世界位置
		FVector Location = GetOwner()->GetActorLocation();
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(
			this, SoundBase, Location);

		if (AudioComponent)
		{
			// 延迟 Duration 秒后停止播放
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle, [AudioComponent]()
				{
					AudioComponent->Stop();
				},
				5, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create audio component for sound playback."));
		}
	}
}
