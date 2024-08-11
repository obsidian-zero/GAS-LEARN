// Fill out your copyright notice in the Description page of Project Settings.


#include "GASLearn/GAS/Global/MyAbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemStats.h"
#include "Engine/Blueprint.h"
#include "GameFramework/Pawn.h"
#include "GameplayCueInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemLog.h"
#include "HAL/LowLevelMemTracker.h"
#include "GameFramework/PlayerController.h"
#include "GameplayCueManager.h"
#include "GameplayTagResponseTable.h"
#include "GameplayTagsManager.h"
#include "Engine/Engine.h"
#include "UObject/UObjectIterator.h"

UGameplayCueManager* UMyAbilitySystemGlobals::GetGameplayCueManager()
{
	if (GlobalGameplayCueManager == nullptr)
	{
		// Load specific gameplaycue manager object if specifieif (GlobalGameplayCueManagerName.IsValid())
		{
			if (GlobalGameplayCueManagerName.IsValid())
			{
				GlobalGameplayCueManager = LoadObject<UGameplayCueManager>(nullptr, *GlobalGameplayCueManagerName.ToString(), nullptr, LOAD_None, nullptr);
				if (GlobalGameplayCueManager == nullptr)
				{
					ABILITY_LOG(Error, TEXT("ERROR to Load GameplayCueManager %s"), *GlobalGameplayCueManagerName.ToString() );
				}
			}
		}

		// Load specific gameplaycue manager class if specified
		if ( GlobalGameplayCueManager == nullptr && GlobalGameplayCueManagerClass.IsValid() )
		{
			UClass* GCMClass = LoadClass<UObject>(NULL, *GlobalGameplayCueManagerClass.ToString(), NULL, LOAD_None, NULL);
			if (GCMClass)
			{
				GlobalGameplayCueManager = NewObject<UGameplayCueManager>(this, GCMClass, NAME_None);
			}
		}

		if ( GlobalGameplayCueManager == nullptr)
		{
			// Fallback to base native class
			GlobalGameplayCueManager = NewObject<UGameplayCueManager>(this, UGameplayCueManager::StaticClass(), NAME_None);
		}

		GlobalGameplayCueManager->OnCreated();

		if (GameplayCueNotifyPaths.Num() == 0)
		{
			GameplayCueNotifyPaths.Add(TEXT("/Game"));
			ABILITY_LOG(Warning, TEXT("No GameplayCueNotifyPaths were specified in DefaultGame.ini under [/Script/GameplayAbilities.AbilitySystemGlobals]. Falling back to using all of /Game/. This may be slow on large projects. Consider specifying which paths are to be searched."));
		}
		
		if (GlobalGameplayCueManager->ShouldAsyncLoadObjectLibrariesAtStart())
		{
			StartAsyncLoadingObjectLibraries();
		}
	}

	check(GlobalGameplayCueManager);
	return GlobalGameplayCueManager;
}