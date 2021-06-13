// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"

// 
class WEATHERSYSTEMEDITOR_API FTextureSaver
{
public:
	FTextureSaver();
	~FTextureSaver();

	// EXPERIMENTAL 
	void CreateTexturePackage(UTextureRenderTarget2D* RenderTarget);

};