// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherSubsystem.h"

void UWeatherSubsystem::CallHTTPRequestError(const FName RequestName)
{
	OnHTTPRequestError.Broadcast(RequestName);
}
