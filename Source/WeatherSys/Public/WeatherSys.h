// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeatherController.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(WeatherSystem, Log, All);


class FWeatherSystemPlugin : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	AWeatherController* SpawnSingletonActor(UWorld* World);

	AWeatherController* GetSingletonActor(UObject* WorldContextObject);

	void EnforceSingletonActor(UWorld* World);

	void InitSingletonActor(UWorld* World, const UWorld::InitializationValues IVS);

	static inline FWeatherSystemPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked<FWeatherSystemPlugin>("WeatherSys");
	}

	static inline bool IsAvaliable()
	{
		return FModuleManager::Get().IsModuleLoaded("WeatherSys");
	}
};
