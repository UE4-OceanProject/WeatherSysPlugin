// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherSystemSettings.h"

UWeatherSystemSettings::UWeatherSystemSettings(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer), DefaultWeatherControllerClass(nullptr)
{
	//LoadConfig()
}

FName UWeatherSystemSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}
#if WITH_EDITOR
FText UWeatherSystemSettings::GetSectionText() const
{
	return NSLOCTEXT("WeatherSystem", "WeatherSystemSettingsSection", "Weather System");
}
#endif