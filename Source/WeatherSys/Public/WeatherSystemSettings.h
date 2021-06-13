// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "WeatherController.h"
#include "UObject/NoExportTypes.h"
#include "WeatherSystemSettings.generated.h"

/**
 * Config varibles for the weather system
 */
UCLASS(config = Engine, defaultconfig, Meta = (DisplayName = "Weather System Settings"))
class WEATHERSYS_API UWeatherSystemSettings : public UDeveloperSettings
{
public:
	GENERATED_BODY()
	

		UWeatherSystemSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

		/** Default Weather controller class to spawn, uses native c++ one if not specified.*/
		UPROPERTY(config, EditAnywhere, Category = Weather)
		TSubclassOf<AWeatherController> DefaultWeatherControllerClass;


		FName GetCategoryName() const override;

#if WITH_EDITOR
		FText GetSectionText() const override;
//		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//
//
//		DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRuntimeMeshComponentSettingsChanged, const FString&, const URuntimeMeshComponentSettings*);
//
//		static FOnRuntimeMeshComponentSettingsChanged& OnSettingsChanged();
//protected:
//	static FOnRuntimeMeshComponentSettingsChanged SettingsChangedDelegate;
#endif

};
