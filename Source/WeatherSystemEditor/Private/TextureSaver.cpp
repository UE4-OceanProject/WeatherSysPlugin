// Copyright Epic Games, Inc. All Rights Reserved.

#include "TextureSaver.h"
#include "AssetRegistryModule.h"

FTextureSaver::FTextureSaver()
{

}
FTextureSaver::~FTextureSaver()
{

}

void FTextureSaver::CreateTexturePackage(UTextureRenderTarget2D* RenderTarget)
{
// EXPERIMENTAL 
	if(!RenderTarget) return;
	TArray<FColor> Data;
	auto RT = RenderTarget->GameThread_GetRenderTargetResource();
	RT->ReadPixels(Data); // Get render target buffer as uint8 FColor


	FString TextureName("TextTex");
	FString PackageName = TEXT("/WeatherSys/WeatherSystem/");
	PackageName += TextureName;
	UPackage* Package = CreatePackage(NULL, *PackageName);
	Package->FullyLoad();

	int32 TextureWidth = RT->GetSizeX();

	UTexture2D* NewTexture = NewObject<UTexture2D>(Package, *TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

	NewTexture->AddToRoot();				// This line prevents garbage collection of the texture
	NewTexture->PlatformData = new FTexturePlatformData();	// Then we initialize the PlatformData
	NewTexture->PlatformData->SizeX = TextureWidth;
	NewTexture->PlatformData->SizeY = TextureWidth;
	NewTexture->PlatformData->SetNumSlices(1);
	NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	

	uint8* Pixels = new uint8[TextureWidth * TextureWidth * 4];
	//for (int32 y = 0; y < TextureWidth; y++)
	//{
	//	for (int32 x = 0; x < TextureWidth; x++)
	//	{
	//		int32 curPixelIndex = ((y * TextureWidth) + x);
	//		Pixels[4 * curPixelIndex] = 128; //b
	//		//Pixels[4 * curPixelIndex + 1] = 64;//G
	//		//Pixels[4 * curPixelIndex + 2] = 32;//R
	//		//Pixels[4 * curPixelIndex + 3] = 16;//A
	//	}
	//}
	for (int32 y = 0; y < TextureWidth/4; y++)
	{
		for (int32 x = 0; x < TextureWidth/4; x++)
		{
			int32 curPixelIndex = ((y * TextureWidth) + x);
			if(curPixelIndex>= Data.Num()) continue;
			Pixels[4 * curPixelIndex] = Data[curPixelIndex].B;
		}
	}


	// Allocate first mipmap.
	FTexture2DMipMap* Mip = new(NewTexture->PlatformData->Mips) FTexture2DMipMap();
	Mip->SizeX = TextureWidth;
	Mip->SizeY = TextureWidth;

	// Lock the texture so it can be modified
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureWidth * TextureWidth * 4);
	FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureWidth * TextureWidth * 4);
	Mip->BulkData.Unlock();

	NewTexture->Source.Init(TextureWidth, TextureWidth, 1, 1, ETextureSourceFormat::TSF_BGRA8, Pixels);

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	NewTexture->UpdateResource();
	Package->MarkPackageDirty();
	AssetRegistryModule.AssetCreated(NewTexture);

	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

	delete[] Pixels;	// Don't forget to free the memory here
}
