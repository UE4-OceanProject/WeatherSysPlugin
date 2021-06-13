// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeatherSystemEditor.h"
#include "EngineUtils.h"

#define LOCTEXT_NAMESPACE "FWeatherSystemEditorPlugin"

DEFINE_LOG_CATEGORY(WeatherSystemEditor);

void FWeatherSystemEditorPlugin::StartupModule()
{
	UE_LOG(WeatherSystemEditor, Log, TEXT("%s:: Module started"), *PLUGIN_FUNC_LINE);
}

void FWeatherSystemEditorPlugin::ShutdownModule()
{

}


IMPLEMENT_MODULE(FWeatherSystemEditorPlugin, WeatherSystemEditor)

