// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeatherSys.h"
#include "WeatherSystemSettings.h"
#include "EngineUtils.h"

#define LOCTEXT_NAMESPACE "FWeatherSystemPlugin"

DEFINE_LOG_CATEGORY(WeatherSystem);

void FWeatherSystemPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}
void FWeatherSystemPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

AWeatherController* FWeatherSystemPlugin::SpawnSingletonActor(UWorld* World)
{
	const UWeatherSystemSettings* WeatherSysConfig = GetMutableDefault<UWeatherSystemSettings>();

	TSubclassOf<AWeatherController> ControllerClass = AWeatherController::StaticClass();
	/* Get class from config if valid and specified. so we can use BP versions*/
	if (WeatherSysConfig && IsValid(WeatherSysConfig->DefaultWeatherControllerClass))
		ControllerClass = WeatherSysConfig->DefaultWeatherControllerClass;

	if (World->bIsRunningConstructionScript)
		return NULL;

	/* Avoid spawning in non relevant worlds such as during packaging where spawning may not be allowed */
	if (World->WorldType == EWorldType::Game || EWorldType::PIE || EWorldType::Editor)
	{
		AWeatherController* WeatherSys = World->SpawnActor<AWeatherController>(ControllerClass, FTransform::Identity, FActorSpawnParameters());
		if (!WeatherSys)
			UE_LOG(WeatherSystem, Display, TEXT("WeatherSystem:: Failed to spawn Singleton!"));

		return WeatherSys;
	}
	else
	{
		return nullptr;
	}
}

AWeatherController* FWeatherSystemPlugin::GetSingletonActor(UObject* WorldContextObject)
{
	if (!WorldContextObject)
		return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	//if (!World->IsGameWorld()) return nullptr;
	for (TActorIterator<AWeatherController> Iter(World); Iter; ++Iter)
	{
		return *Iter; // return first, should only ever be 1 anyway
	}

	/* If we get here then there is no weather controller in the map, so spawn one */
	AWeatherController* NewController = SpawnSingletonActor(World);

	if (NewController) return NewController;

	/* If it failed to sapwn then it will log out error in SpawnSingletonActor() and this will return null */
	return nullptr;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FWeatherSystemPlugin, WeatherSys)