// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "WeatherDataTypes.generated.h"


UENUM(BlueprintType)
enum class EWeatherAPICallbackStatus : uint8
{
	E_Ok UMETA(DisplayName = "OK"),
	E_APIOffline UMETA(DisplayName = "API Offline"),
	E_FailedToFindCity UMETA(DisplayName = "Failed to find City"),
	E_APICallsExceeded UMETA(DisplayName = "API calls limit exceeded"),
	E_InvalidResponseObject UMETA(DisplayName = "Invalid response object"),
	E_UnknownError UMETA(DisplayName = "Unknown error")

};


UENUM(BlueprintType)
enum class EOpenWeatherAPIQueryType : uint8
{
	E_CityLocation,
	E_LatLong

};

/* human readable weather condition */
UENUM(BlueprintType)
enum class EWeatherCondition : uint8
{
	W_Min UMETA(Hidden),
	W_Clear UMETA(DisplayName = "Clear"),
	W_PartlyCloudy UMETA(DisplayName = "Partially Cloudy"),
	W_Overcast UMETA(DisplayName = "Overcast Cloud"),
	W_LightRain UMETA(DisplayName = "Light Rain"),
	W_Raining UMETA(DisplayName = "Raining"),
	W_ThunderStorm UMETA(DisplayName = "Thunder Storm"),
	W_Hail UMETA(DisplayName = "Hail"),
	W_Snowing UMETA(DisplayName = "Snowing"),
	W_Max  UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ESeason : uint8
{
	E_Summer UMETA(DisplayName = "Summer"),
	E_Autumn UMETA(DisplayName = "Autumn"),
	E_Winter UMETA(DisplayName = "Winter"),
	E_Spring UMETA(DisplayName = "Spring")
};

/** Single	Point based weather conditions
 * 
 */
 USTRUCT(Blueprintable, BlueprintType)
 struct WEATHERSYS_API FWeatherStateData
 {
	 GENERATED_USTRUCT_BODY()
 

	/* Normalized global wind direction */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector WindDirection;

	 /* Wind Direction In Degrees From North, 0 is north, 90 is east etc */
	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 float WindDirectionInDegreesFromNorth;

	/* Wind speed in km/h, (Multiply by 27.7778f  to convert to UE4 cm/s) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindSpeed;

	/* Max wind gust speed in Km/h  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindGustSpeed;


	/* Normalized global Sun Direction. If Z is Greater than 0 then sun is bellow horizon. To convert to a world rotation, create a rotator from X */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SunDirection;

	/* Air humidity as a percentage, from 0 - 100% */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	int32 Humidity;

	/*  Air Temperature in Celsius at Sealevel */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = -100, ClampMax = 150, UIMin = -100, UIMax = 150))
	float SeaLevelTemperature;

	/* Temp at which air can become water saturated, if air temp is at this point and comes in contact with a cooler surface then condensation/dew will form*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = -100, ClampMax = 150, UIMin = -100, UIMax = 150))
	float DewPoint;

	/* Low level mist/fog intensity, 0.0-1.0. Separate from Visibility distance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float FogAmount;


	/* Rate of Precipitation (rain/snow fall rate) in 0-100 . Probably need a manual override control for it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	float PrecipitationRate;

	/* Amount of snow in mm past 3hours  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	float SnowFallPast3Hrs;


	/* Air pressure in hpa, Placeholder incase we need it  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 2000, UIMin = 0, UIMax = 2000))
	float AirPressure;

	/* View distance in KM, relates to air clarity/haze density  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0, ClampMax = 200, UIMin = 0, UIMax = 200))
	float VisibilityDistance;

	FWeatherStateData()
	{
		WindDirection = FVector::ZeroVector;
		WindDirectionInDegreesFromNorth = 0.f;
		WindSpeed = 0.f;
		WindGustSpeed = 0.f;
		SunDirection = FVector::ZeroVector;
		Humidity = 0;
		SeaLevelTemperature = 19.f;
		DewPoint = 9.f;
		FogAmount = 0.f;
		PrecipitationRate = 0.f;
		SnowFallPast3Hrs = 0.f;
		AirPressure = 1000.f;
		VisibilityDistance = 20.f;
	
	}

 };