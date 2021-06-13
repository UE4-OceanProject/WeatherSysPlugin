// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeatherSys.h"
#include "WeatherSystemSettings.h"
#include "EngineUtils.h"

#define LOCTEXT_NAMESPACE "FWeatherSystemPlugin"

DEFINE_LOG_CATEGORY(LogWeatherSystem);

void FWeatherSystemPlugin::StartupModule()
{
	UE_LOG(LogWeatherSystem, Display, TEXT("%s:: Module started"), *PLUGIN_FUNC_LINE);
}

void FWeatherSystemPlugin::ShutdownModule()
{
}

void FWeatherSystemPlugin::EnforceSingletonActor(UWorld* World)
{
	//Make sure there is only one instance of this actor!
	//Actor is not blueprintable, but users will find other ways!!
	bool bFoundFirstInstance = false;
	for (TActorIterator<AWeatherController> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (bFoundFirstInstance == false)
		{
			bFoundFirstInstance = true;
		}
		else
		{
			UE_LOG(LogWeatherSystem, Display, TEXT("%s:: found more than one WeatherSystem destroying..."), *PLUGIN_FUNC_LINE);
			ActorItr->Destroy();
		}
	}
}

AWeatherController * FWeatherSystemPlugin::SpawnSingletonActor(UWorld* World)
{
	const UWeatherSystemSettings* WeatherSysConfig = GetMutableDefault<UWeatherSystemSettings>();
	TSubclassOf<AWeatherController> ControllerClass = AWeatherController::StaticClass();
	/* Get class from config if valid and specified. so we can use BP versions*/
	if (WeatherSysConfig && IsValid(WeatherSysConfig->DefaultWeatherControllerClass))
		ControllerClass = WeatherSysConfig->DefaultWeatherControllerClass;

	AWeatherController* WeatherSys = World->SpawnActor<AWeatherController>(ControllerClass, FTransform::Identity, FActorSpawnParameters());
	if (!WeatherSys)
		UE_LOG(LogWeatherSystem, Display, TEXT("%s:: Failed to spawn Singleton!"), *PLUGIN_FUNC_LINE);

	return WeatherSys;
}

AWeatherController * FWeatherSystemPlugin::GetSingletonActor(UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject->GetWorld();

	//Interesting issue possibly, we get the default class from editor settings
//but what if the pre-existing controller is a different class
//how is that handled?
	if ((World->WorldType == EWorldType::EditorPreview) || (World->WorldType == EWorldType::GamePreview))
		return NULL;
	if (World->bIsRunningConstructionScript)
		return NULL;

	EnforceSingletonActor(World);

	for (TActorIterator<AWeatherController> ActorItr(World); ActorItr; ++ActorItr)
	{
		return *ActorItr;
	}

	//In the impossible case that we don't have an actor, spawn one!
	UE_LOG(LogWeatherSystem, Display, TEXT("%s:: No WeatherSys found... spawning..."), *PLUGIN_FUNC_LINE);
	return SpawnSingletonActor(World);
}

IMPLEMENT_MODULE(FWeatherSystemPlugin, WeatherSys)

