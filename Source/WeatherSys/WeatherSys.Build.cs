// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class WeatherSys : ModuleRules
{
	public WeatherSys(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

       
        PublicIncludePaths.AddRange(
			new string[] {
                Path.Combine(ModuleDirectory,"Public"),
                Path.Combine(ModuleDirectory,"Private") // added private include here so other plugins can access WfuncionLib, could shift the file into public folder.....
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                Path.Combine(ModuleDirectory,"Private")
                
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			   "TimePlugin",
			   "DeveloperSettings"//4.26
				// ... add other public dependencies that you statically link with here ...
			}
			);
       
        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"VaRest"
				
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
