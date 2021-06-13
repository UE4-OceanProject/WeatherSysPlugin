
#pragma once

#include "WeatherSys.h"
#include "WeatherController.h"
#include "WeatherFunctionLibrary.generated.h"

UCLASS()
class WEATHERSYS_API UWeatherSystemFunctionLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Weather System", Keywords = ""), meta = (WorldContext = "WorldContextObject"), Category = "Weather System")
		static AWeatherController* GetWeatherSystem(UObject* WorldContextObject)
	{
		return FWeatherSystemPlugin::Get().GetSingletonActor(WorldContextObject);
	}

	/* Convert windspeed in km/h to engines cm/s*/
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Weather System|Utility")
	static float GetWindSpeedInCMS(const float InKMhSpeed)
	{
		return InKMhSpeed * 27.7778f;
	}

};
