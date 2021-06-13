// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class WeatherSystemEditor : ModuleRules
{
	public WeatherSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

       
        PublicIncludePaths.AddRange(new string[] {
                Path.Combine(ModuleDirectory,"Public"),
                Path.Combine(ModuleDirectory,"Private") // added private include here so other plugins can access WfuncionLib, could shift the file into public folder.....
				// ... add public include paths required here ...
			});
				
		
		PrivateIncludePaths.AddRange(new string[] {
                Path.Combine(ModuleDirectory,"Private")

			});
			
		
		PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
				"AssetRegistry"

			});
       
        PrivateDependencyModuleNames.AddRange(new string[] {
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			});
		
		
		DynamicallyLoadedModuleNames.AddRange(new string[] {
				// ... add any modules that your module loads dynamically here ...
			});
	}
}
