// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
#include "WeatherController.h"
//#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWeatherSystem, Display, All);

#define PLUGIN_FUNC (FString(__FUNCTION__))              // Current Class Name + Function Name where this is called
#define PLUGIN_LINE (FString::FromInt(__LINE__))         // Current Line Number in the code where this is called
#define PLUGIN_FUNC_LINE (PLUGIN_FUNC + "(Line: " + PLUGIN_LINE + ")") // Current Class and Line Number where this is called!


class WEATHERSYS_API  FWeatherSystemPlugin : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	AWeatherController* SpawnSingletonActor(UWorld* World);

	AWeatherController* GetSingletonActor(UObject* WorldContextObject);

	void EnforceSingletonActor(UWorld* World);
	

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FWeatherSystemPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< FWeatherSystemPlugin >("WeatherSys");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("WeatherSys");
	}
};

