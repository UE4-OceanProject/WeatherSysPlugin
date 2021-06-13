// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextureSaver.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "WeatherSystemEditorLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WEATHERSYSTEMEDITOR_API UWeatherSystemEditorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Weather System")
	static void MakeTexture(UTextureRenderTarget2D* RenderTarget)
	{
		TSharedPtr<FTextureSaver> TextureSaver = MakeShared<FTextureSaver>();

		TextureSaver->CreateTexturePackage(RenderTarget);
		TextureSaver.Reset();
	}
};
