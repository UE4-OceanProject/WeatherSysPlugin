// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WeatherSubsystem.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHTTPRequestError, FName, RequestName);
/** Place holder in weather for the moment, but i think something like this should be in something more primary
 * 
 */
UCLASS()
class WEATHERSYS_API UWeatherSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	/* Called in response to CallHTTPRequestError() when there is a network error from VA REST to notify UI etc */
	UPROPERTY(BlueprintAssignable)
	FOnHTTPRequestError OnHTTPRequestError;

	/* Call the  */
	UFUNCTION(BlueprintCallable)
	void CallHTTPRequestError(const FName RequestName);
};
