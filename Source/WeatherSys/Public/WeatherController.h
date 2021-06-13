// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "TimeDateStruct.h"

#include "WeatherDataTypes.h"
#include "WeatherController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeatherCondition, EWeatherCondition, NewCondition, FWeatherStateData, WeatherStateData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeatherAPICallBack, EWeatherAPICallbackStatus, Response);



UCLASS()
class UOpenWeatherAPI : public UObject
{
public:
	//#TODO Polymorphism
	GENERATED_BODY()

		UOpenWeatherAPI();


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString APIKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString BaseURL;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QueryString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString City;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CityCountryCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EOpenWeatherAPIQueryType QueryType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = -90, UIMax = 90))
		float Latitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = -180, UIMax = 180))
		float Longitude;

	/* Returns formatted URL for OpenWeather API*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual	FString GetURL()
	{
		FString URL;
		FString Lat, Long;
		//If float to int conversion was intentional use: 
		//Lat.AppendInt(Latitude);
		//Long.AppendInt(Longitude);
		Long = FString::SanitizeFloat(Longitude);
		Lat = FString::SanitizeFloat(Latitude);
		switch (QueryType)
		{
		case EOpenWeatherAPIQueryType::E_CityLocation: {
			QueryString = FString("weather?q=");
			return URL += BaseURL + QueryString + City + "," + CityCountryCode + "&" + "appid=" + APIKey;
		}
													 break;
		case EOpenWeatherAPIQueryType::E_LatLong: {
			QueryString = FString("weather?") + FString("lat=") + Lat + "&" + "lon=" + Long + "&";
			return URL += BaseURL + QueryString + "&" + "appid=" + APIKey;
		}
												break;
		}
		return URL += BaseURL + QueryString + City + "," + CityCountryCode + "&" + "appid=" + APIKey;
	}

};

class UCharacterMovementComponent;
/* Weather Controller Actor
* Forward vector of this actor should always face north
*/
UCLASS(BlueprintType)
class WEATHERSYS_API AWeatherController : public AActor
{
	GENERATED_BODY()

		/** Default UObject constructor. */
		AWeatherController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void OnConstruction(const FTransform& Transform) override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

	/* Single point weather state information, either assigned from weatherAPI or user input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather System")
		FWeatherStateData WeatherData;

	/*UPROPERTY(BlueprintReadOnly, Category = "Weather System|Procedural gen")
	URuntimeMeshProviderStatic* StaticProvider;*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UArrowComponent* WindDirectionIndicatorComponent;

	/* #TODO Implement when searching for actors to control the materials of */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* ControlRegion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWindDirectionalSourceComponent* SpeedTreeWindComponent;

public:

	/* Array of Dynamic Mat instances of each object detected in the begin search and added to the controlled list */
	UPROPERTY()
		TArray<UMaterialInstanceDynamic*> DynamicMats;


	/*
	* Actors to exclude from begin search.
	* Searches map on begin play to find actors in which it should manage the materials of, eg snow amount, wetness etc #TODO master materials with shared names on variables */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weather System|Material interface")
		FName ActorSearchExcludeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeatherCondition WeatherCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 bIsNorthernHemisphere : 1;

	/* Required humidity level for precepication to start, usually 95 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather System")
		int32 RequiredPrecipitationHumidity;

	/* Set a float value within all controlled material instances */
	UFUNCTION(BlueprintCallable, Category = "Weather System|Utility")
		void SetScalarMaterialProperty(const FName PropertyName, const float NewValue);

	/*  Manually change the weather condition
	@param NewCondition , manual weather condition desired
	@param NewStateData , State of the new weather data, should match the condition set
	*/
	UFUNCTION(BlueprintCallable, Category = "Weather System")
		void ChangeWeatherCondition(const EWeatherCondition NewCondition, const FWeatherStateData NewStateData);

	/* called when weather condition changes state, internal blueprint only, use delegate OnWeatherConditionChanged , for external use */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weather System")
		void BP_WeatherConditionChanged();

	/* called when weather condition changes state, internal blueprint only, use delegate OnWeatherConditionChanged , for external use */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weather System")
		void BP_TimeChanged();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weather System")
		void GetCurrentWeather(EWeatherCondition& CurrentCondition, FWeatherStateData& CurrentData);

	/* Determines the current condition from the WeatherData variable */
	UFUNCTION(BlueprintCallable, Category = "Weather System")
		EWeatherCondition DertermineCurrentCondition();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weather System")
		UMaterialParameterCollection* GetSeasionParameters()const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weather System")
		UMaterialParameterCollection* GetMaterialParameters()const;

	/* Returns current season we are in, takes into account northern/southern hemisphere. Based off TimePlugin */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weather System|Season")
		ESeason GetCurrentSeason() const;

	/* ======== DELEGATES ====== */
	/* Called when a new weather condition is manually set or theres a large change in conditions that would cause a change, e.g, temp drops and air pressure drops causing clouds/precip */
	UPROPERTY(BlueprintAssignable, Category = "Weather System")
		FOnWeatherCondition OnWeatherConditionChanged;

	/* Called when the weather api has responded or when a time out was reached.  */
	UPROPERTY(BlueprintAssignable, Category = "Weather System")
		FOnWeatherAPICallBack OnWeatherAPICallback;






	/* Call when time has been changed from time manager
	* @param NewTime The new current time
	*/
	UFUNCTION(BlueprintCallable, Category = "Weather System")
		void OnTimeChanged(FTimeDate NewTime);

	UFUNCTION(BlueprintCallable, Category = "Weather System")
		virtual void UpdateWeatherFromAPI_CityLocation(const FString CityName, const FString CountryCode);

	/* Get weather data from lat long coords,
	@param Lat : if your Latitude is in the southern hemisphere add a Negative in front like -42.6
	@param Long : if your Longitude says West after it eg, 130.5w then add a negative in front like - 130.5
	*/
	UFUNCTION(BlueprintCallable, Category = "Weather System")
		virtual void UpdateWeatherFromAPI_LatLong(const float Lat, const float Long);

	/* Internal callback */
	UFUNCTION()
		void OnAPIResponse(UVaRestRequestJSON* Request);

	/* Called when the Weather API responds to the HTTP query  */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weather System")
		void BP_OnWeatherAPIResponse(UVaRestRequestJSON* Request, EWeatherAPICallbackStatus APIResponse);

private:

	/** Seasons Parameter collections */
	UPROPERTY(EditDefaultsOnly, Category = "Weather System|Material interface")
		UMaterialParameterCollection* SeasonParameters;

	/** Material Parameter collection for snow,precipitation,wetness etc */
	UPROPERTY(EditDefaultsOnly, Category = "Weather System|Material interface")
		UMaterialParameterCollection* MaterialParameters;

	/** Material Parameter collection for snow,precipitation,wetness etc */
	UPROPERTY(EditDefaultsOnly, Category = "Weather System")
		TSubclassOf<UOpenWeatherAPI> WeatherAPI;

	UPROPERTY()
		UOpenWeatherAPI* InternalAPIPtr;

	/** Current Season, takes into account northern or southern hemisphere */
	ESeason Season;

	void UpdateSpeedTreeWind();

	/* North facing rotation of the actor */
	UPROPERTY(EditDefaultsOnly)
		FRotator RotationOffset;
};
