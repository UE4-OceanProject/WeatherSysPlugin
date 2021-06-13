// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherController.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/ExponentialHeightFog.h"
#include "DrawDebugHelpers.h"

#include "Engine/WindDirectionalSource.h"
#include "Components/WindDirectionalSourceComponent.h"

//#include "TrueSkySequenceActor.h"
#include "VaRest/Public/VaRestSubsystem.h"
#include "VaRest/Public/VaRestJsonObject.h"


#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"


UOpenWeatherAPI::UOpenWeatherAPI()
{
	APIKey = FString("4719e3425c647300b56f6a5601621a3e");
	BaseURL = FString("api.openweathermap.org/data/2.5/");
	QueryString = FString("weather?");
	City = FString("Auckland");
	CityCountryCode = FString("nz");
	QueryType = EOpenWeatherAPIQueryType::E_CityLocation;

}


// Sets default values
AWeatherController::AWeatherController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SnowFallAmount = 1.f;
	//SlipAngle= 10.f;
	//GridDivisions = 4.f;
	//SnowMeshNavOffset = -50.f;
	RequiredPrecipitationHumidity = 95;
	WeatherData = FWeatherStateData();
	SeasonParameters = nullptr;
	MaterialParameters = nullptr;
	Season = ESeason::E_Summer;
	bIsNorthernHemisphere = true;
	RotationOffset = FRotator(0, 90, 0);
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (Root)
	{
		SetRootComponent(Root);
	}

	WindDirectionIndicator = CreateDefaultSubobject<UArrowComponent>(TEXT("Wind Direction"));
	if (WindDirectionIndicator)
	{
		WindDirectionIndicator->SetupAttachment(GetRootComponent());
		WeatherData.WindDirection = WindDirectionIndicator->GetForwardVector(); // flows in direction arrows heading in
	}

	ControlRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("Region Control Box"));
	if (ControlRegion)
	{
		ControlRegion->SetupAttachment(GetRootComponent());
		ControlRegion->SetCollisionObjectType(ECC_WorldDynamic);
		//ControlRegion->SetCollisionResponseToAllChannels(ECR_Ignore);
		ControlRegion->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);

	}

	SpeedTreeWind = CreateDefaultSubobject<UChildActorComponent>(TEXT("Speed Tree Wind Source"));
	if (SpeedTreeWind)
	{
		SpeedTreeWind->SetupAttachment(GetRootComponent());
		SpeedTreeWind->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(WeatherData.WindDirection).Quaternion());

	}

	//if(!bUseCustomHeightFog)
	//{
	//	/*HeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("Height fog"));
	//	if(HeightFog)
	//	{
	//		HeightFog->SetupAttachment(GetRootComponent());
	//	}*/
	//}


}

void AWeatherController::OnConstruction(const FTransform& Transform)
{
	FTransform ModRot = FTransform::Identity;
	ModRot.SetRotation(RotationOffset.Quaternion());

	/* Rotation must always face north */
	//SetActorRotation(RotationOffset.Quaternion());
	SpeedTreeWind->SetChildActorClass(AWindDirectionalSource::StaticClass());
	SpeedTreeWind->CreateChildActor();
	if (!IsValid(InternalAPIPtr) && WeatherAPI->IsValidLowLevel()) InternalAPIPtr = NewObject<UOpenWeatherAPI>(this, WeatherAPI);
	UpdateSpeedTreeWind();
	Super::OnConstruction(Transform);
}

//Need to duplicate the child actor setup in both construction and initialize components. One allows us to change the wind, 
//the other allows us to run our packaged game. Weird.
void AWeatherController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	/* #TODO implement properly */
	if (ControlRegion)
	{
		//TSet<AActor*> Actors;
		//ControlRegion->GetOverlappingActors(Actors, AStaticMeshActor::StaticClass());
		/* Not Needed atm */
		//if(GetWorld())
		//{
		//	TArray<AActor*> Actors;
		//	// Have to use this Bull Track method cos TActorIterator is being stupid
		//	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AStaticMeshActor::StaticClass(), Actors);
		//	for(auto& Elem: Actors)
		//	{
		//		if (AStaticMeshActor* Act = Cast<AStaticMeshActor>(Elem))
		//		{
		//			if(Act->ActorHasTag(ActorSearchExcludeTag)) continue;
		//			int32 NumMaterials = 0;
		//			//Disabled for quest 
		//			//NumMaterials = Act->GetStaticMeshComponent()->GetNumMaterials();
		//			NumMaterials = 0;

		//			UMaterialInstanceDynamic* Mat = nullptr;
		//			for(int32 i = 0; i<NumMaterials; i++)
		//			{
		//				//Disabled for quest 
		//				//Mat = Act->GetStaticMeshComponent()->CreateDynamicMaterialInstance(i, Act->GetStaticMeshComponent()->GetMaterial(i));
		//				Mat = NULL;

		//			}
		//			if(Mat)
		//				DynamicMats.Add(Mat); // #TODO Material detection / inclusion
		//		}			
		//	}		
		//}	
		//ControlRegion->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	}


	/* Basic implem of startup weather condition, TODO get data from remote server  */
	//WeatherCondition = DertermineCurrentCondition();
	//OnWeatherConditionChanged.Broadcast(WeatherCondition, WeatherData);
}

// Called every frame
void AWeatherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeatherController::BeginDestroy()
{
	Super::BeginDestroy();
}

void AWeatherController::SetScalarMaterialProperty(const FName PropertyName, const float NewValue)
{
	for (UMaterialInstanceDynamic* Elem : DynamicMats)
	{
		//Disabled for quest
		//Elem->SetScalarParameterValue(PropertyName, NewValue);
	}
}

EWeatherCondition AWeatherController::DertermineCurrentCondition()
{
	EWeatherCondition NewCondition = EWeatherCondition::W_Clear;
	//#TODO Simple implementation currently


	/* Probably snowing at surface level */
	if (WeatherData.SeaLevelTemperature < 1 && WeatherData.Humidity > RequiredPrecipitationHumidity)
	{
		NewCondition = EWeatherCondition::W_Snowing;
	}
	else if (WeatherData.SeaLevelTemperature > 1 && WeatherData.Humidity > RequiredPrecipitationHumidity)
		NewCondition = EWeatherCondition::W_Raining;
	/* Dial up precipitation rate  on controlled particle systems etc when it starts to rain/snow */

	return NewCondition;
}

UMaterialParameterCollection* AWeatherController::GetSeasionParameters() const
{
	return SeasonParameters;
}

UMaterialParameterCollection* AWeatherController::GetMaterialParameters() const
{
	return MaterialParameters;
}

ESeason AWeatherController::GetCurrentSeason() const
{
	return Season;
}

void AWeatherController::OnTimeChanged(FTimeDate NewTime)
{
	//#TODO Update scenery and material parameters with new season information
	   //UE_LOG(LogTemp,Warning, TEXT("Weather Time changed"));

	if (NewTime.Month < 3)
	{
		if (bIsNorthernHemisphere)
			Season = ESeason::E_Winter;
		else
			Season = ESeason::E_Summer;
	}
	else if (NewTime.Month < 6)
	{
		if (bIsNorthernHemisphere)
			Season = ESeason::E_Spring;
		else
			Season = ESeason::E_Autumn;
	}
	else if (NewTime.Month < 9)
	{
		if (bIsNorthernHemisphere)
			Season = ESeason::E_Summer;
		else
			Season = ESeason::E_Winter;
	}
	else if (NewTime.Month < 11)
	{
		if (bIsNorthernHemisphere)
			Season = ESeason::E_Autumn;
		else
			Season = ESeason::E_Spring;
	}
	else if (NewTime.Month == 12)
	{
		if (bIsNorthernHemisphere)
			Season = ESeason::E_Winter;
		else
			Season = ESeason::E_Summer;
	}
}

void AWeatherController::UpdateWeatherFromAPI_CityLocation(const FString CityName, const FString CountryCode)
{
	if (GetWorld()->IsGameWorld())
	{
		checkf(GEngine, TEXT("UpdateWeatherFromAPI_LatLong:: Invalid GEngine ptr"));
		checkf(InternalAPIPtr, TEXT("UpdateWeatherFromAPI_LatLong:: Invalid WeatherAPI ptr"));
		UVaRestSubsystem* VASub = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
		if (!VASub) return;
		InternalAPIPtr->City = CityName; InternalAPIPtr->CityCountryCode = CountryCode; InternalAPIPtr->QueryType = EOpenWeatherAPIQueryType::E_CityLocation;

		//UE_LOG(LogTemp,Display,TEXT("%s"),*InternalAPIPtr->GetURL());
		FVaRestCallDelegate OnCallback;
		OnCallback.BindUFunction(this, "OnAPIResponse");
		VASub->CallURL(InternalAPIPtr->GetURL(), EVaRestRequestVerb::CUSTOM, EVaRestRequestContentType::x_www_form_urlencoded_url, nullptr, OnCallback);
	}
}

void AWeatherController::UpdateWeatherFromAPI_LatLong(const float Lat, const float Long)
{
	if (GetWorld()->IsGameWorld())
	{
		checkf(GEngine, TEXT("UpdateWeatherFromAPI_LatLong:: Invalid GEngine ptr"));
		checkf(InternalAPIPtr, TEXT("UpdateWeatherFromAPI_LatLong:: Invalid WeatherAPI ptr"));
		UVaRestSubsystem* VASub = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
		if (!VASub) return;
		InternalAPIPtr->Latitude = Lat; InternalAPIPtr->Longitude = Long; InternalAPIPtr->QueryType = EOpenWeatherAPIQueryType::E_LatLong;

		FVaRestCallDelegate OnCallback;
		OnCallback.BindUFunction(this, "OnAPIResponse");
		VASub->CallURL(InternalAPIPtr->GetURL(), EVaRestRequestVerb::CUSTOM, EVaRestRequestContentType::x_www_form_urlencoded_url, nullptr, OnCallback);
	}
}

void AWeatherController::OnAPIResponse(UVaRestRequestJSON* Request)
{
	/* Potentially move this into the UWeatherAPI object as a virtual so we
	can use this for different API's  */
	if (!IsValid(Request) || !Request->bIsValidJsonResponse)
	{
		OnWeatherConditionChanged.Broadcast(WeatherCondition, WeatherData);

		OnWeatherAPICallback.Broadcast(EWeatherAPICallbackStatus::E_UnknownError);
		BP_OnWeatherAPIResponse(Request, EWeatherAPICallbackStatus::E_UnknownError);
		return;
	}
	int32 ResponseCode = INDEX_NONE;
	/* Main Data Response */
	if (UVaRestJsonObject* JsonResp = Request->GetResponseObject())
	{
		/*===  Error Responses  === */
		if (JsonResp->HasField("cod"))
		{	/* Response code is a string field not number.....why....*/
			ResponseCode = UKismetStringLibrary::Conv_StringToInt(JsonResp->GetStringField("cod"));
		}
		if (ResponseCode != 200) // 200 is their OK response, #TODO Expose as variable/macro for different API's codes
		{
			if (ResponseCode == 429) // Exceeded call limit
			{
				OnWeatherConditionChanged.Broadcast(WeatherCondition, WeatherData);
				OnWeatherAPICallback.Broadcast(EWeatherAPICallbackStatus::E_APICallsExceeded);
				BP_OnWeatherAPIResponse(Request, EWeatherAPICallbackStatus::E_APICallsExceeded);
				return;
			}
			else if (ResponseCode == 404) // City not found
			{
				OnWeatherConditionChanged.Broadcast(WeatherCondition, WeatherData);
				OnWeatherAPICallback.Broadcast(EWeatherAPICallbackStatus::E_FailedToFindCity);
				BP_OnWeatherAPIResponse(Request, EWeatherAPICallbackStatus::E_FailedToFindCity);
				return;
			}
		}
		/* ======================== */

		/* Get Main data field struct */
		if (UVaRestJsonObject* MainField = JsonResp->GetObjectField("main"))
		{
			WeatherData.SeaLevelTemperature = MainField->GetNumberField("temp") - 273.15; // convert from kelvin to Celsius, API gives it in kelvin
			WeatherData.Humidity = (int32)MainField->GetNumberField("humidity");
			if (MainField->HasField("pressure"))
				WeatherData.AirPressure = MainField->GetNumberField("pressure");
		}
		/* Get wind field */
		if (UVaRestJsonObject* WindField = JsonResp->GetObjectField("wind"))
		{
			WeatherData.WindSpeed = WindField->GetNumberField("speed") * 3.6; // convert from meters/s to km/h		
			if (WindField->HasField("gust"))
				WeatherData.WindGustSpeed = WindField->GetNumberField("gust");

			/* Wind direction is described in degrees, where a wind blowing FROM north to south is 0"
			A wind blowing FROM the east is 90" an so on
			*/
			float WindDegrees = 0.f;
			if (WindField->HasField("deg"))
				WindDegrees = WindField->GetNumberField("deg");

			FVector NewDir = GetActorTransform().GetRotation().GetForwardVector();
			NewDir = NewDir.RotateAngleAxis(WindDegrees, GetActorTransform().GetRotation().GetUpVector());

			/* Update wind actors */
			//ATM Arrows are in DIRECTION of the wind, not the direction its COMING from and going to
			WeatherData.WindDirectionInDegreesFromNorth = WindDegrees;
			WeatherData.WindDirection = NewDir;
			if (WindDirectionIndicator)
				WindDirectionIndicator->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(NewDir));
			if (SpeedTreeWind)
			{
				SpeedTreeWind->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(NewDir));
				UpdateSpeedTreeWind();
			}
		}
		// Get visibility
		if (JsonResp->HasField("visibility"))
			WeatherData.VisibilityDistance = JsonResp->GetNumberField("visibility");
		/* Get Rain */
		if (UVaRestJsonObject* RainField = JsonResp->GetObjectField("rain"))
		{
			float RainPastHr = RainField->GetNumberField("1h");
			float RainPast3Hr = RainField->GetNumberField("3h");
			// this is arbitrary atm
			WeatherData.PrecipitationRate = FMath::Clamp(RainPastHr + RainPast3Hr * 10.f, 0.f, 100.f);
		}
		/* Get Rain */
		if (UVaRestJsonObject* SnowField = JsonResp->GetObjectField("snow"))
		{
			float SnowPastHr = SnowField->GetNumberField("1h");
			float SnowPast3Hr = SnowField->GetNumberField("3h");

			WeatherData.SnowFallPast3Hrs = FMath::Clamp(SnowPastHr + SnowPast3Hr, 0.f, 100.f);
		}

		/* Weather field uses a array of 1....... why, i do not know... */
		/* Description of current conditions   */
		TArray<UVaRestJsonObject*> weatherField = JsonResp->GetObjectArrayField("weather");
		for (auto Elem : weatherField)
		{
			if (Elem->HasField("main"))
			{
				FString Description = Elem->GetStringField("description");
				FString Conditions = Elem->GetStringField("main");
				if (Conditions == "Clear")
					WeatherCondition = EWeatherCondition::W_Clear;
				else if (Conditions == "Clouds")
				{
					if (Description.Contains("Broken") || Description.Contains("Scattered"))
						WeatherCondition = EWeatherCondition::W_PartlyCloudy;
					else if (Description.Contains("Overcast"))
						WeatherCondition = EWeatherCondition::W_Overcast;
				}
				else if (Conditions == "Rain")
					WeatherCondition = EWeatherCondition::W_Raining;
				else if (Conditions == "light Rain")
					WeatherCondition = EWeatherCondition::W_LightRain;
				else if (Conditions == "mist")
				{
					WeatherCondition = EWeatherCondition::W_Overcast;
					WeatherData.FogAmount = 0.5f;
				}
				else
				{
					WeatherCondition = EWeatherCondition::W_Clear;
				}
			}
			UE_LOG(LogTemp, Display, TEXT("Weather Conditions= %s"), *Elem->GetStringField("main"));
		}

		OnWeatherConditionChanged.Broadcast(WeatherCondition, WeatherData);
		OnWeatherAPICallback.Broadcast(EWeatherAPICallbackStatus::E_Ok);

		BP_OnWeatherAPIResponse(Request, EWeatherAPICallbackStatus::E_Ok);
	}
	else
	{
		OnWeatherConditionChanged.Broadcast(WeatherCondition, WeatherData);
		OnWeatherAPICallback.Broadcast(EWeatherAPICallbackStatus::E_InvalidResponseObject);
		BP_OnWeatherAPIResponse(Request, EWeatherAPICallbackStatus::E_InvalidResponseObject);
		return;
	}
}

void AWeatherController::UpdateSpeedTreeWind()
{
	auto test = SpeedTreeWind->GetChildActor();
	if (AWindDirectionalSource* ST_Wind = Cast<AWindDirectionalSource>(SpeedTreeWind->GetChildActor()))
	{/* set the wind parameters into speed tree from weather data*/

		// Speed tree uses a odd value for its wind speed, reducing the KM/H wind  speed by a factor of 10 seems to give a reasonable speed result on trees
		//Pretty sure its a 0-1 value. Would need to setup a range converter
		const float W_Speed = WeatherData.WindSpeed * 0.07f;
		const float W_Gust = WeatherData.WindGustSpeed * 0.07f;
		ST_Wind->GetComponent()->SetStrength(W_Speed);
		ST_Wind->GetComponent()->SetSpeed(W_Speed);
		ST_Wind->GetComponent()->SetMinimumGustAmount(W_Speed);
		ST_Wind->GetComponent()->SetMaximumGustAmount(W_Gust);
	}
}

void AWeatherController::ChangeWeatherCondition(const EWeatherCondition NewCondition, const FWeatherStateData NewStateData)
{
	WeatherCondition = NewCondition;
	WeatherData = NewStateData;
	OnWeatherConditionChanged.Broadcast(WeatherCondition, WeatherData);

	if (WeatherData.WindDirection == FVector::ZeroVector)
		WeatherData.WindDirection = FVector(1, 0, 0); // dont want a zero vector for direction

	//WeatherData.SunDirection = UKismetMathLibrary::GetForwardVector(ATrueSkySequenceActor::GetSunRotation());
	const FRotator WindDir = UKismetMathLibrary::MakeRotFromX(WeatherData.WindDirection);
	if (WindDirectionIndicator)
		WindDirectionIndicator->SetWorldRotation(WindDir);
	if (SpeedTreeWind)
	{
		SpeedTreeWind->SetWorldRotation(WindDir);
		UpdateSpeedTreeWind();
	}

	BP_WeatherConditionChanged();
}

void AWeatherController::GetCurrentWeather(EWeatherCondition& CurrentCondition, FWeatherStateData& CurrentData)
{
	CurrentCondition = WeatherCondition;
	CurrentData = WeatherData;
}

