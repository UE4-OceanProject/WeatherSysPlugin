using UnrealBuildTool;
using System.IO;

public class WeatherSys : ModuleRules
{
	public WeatherSys(ReadOnlyTargetRules Target) : base(Target)
	{
        PublicIncludePaths.AddRange(new string[] {
            Path.Combine(ModuleDirectory,"Public"),
            Path.Combine(ModuleDirectory,"Private") // added private include here so other plugins can access WfuncionLib, could shift the file into public folder...
		});				
		
		PrivateIncludePaths.AddRange(new string[] {
            Path.Combine(ModuleDirectory,"Private")
		});			
		
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"TimePlugin"
		});
       
        PrivateDependencyModuleNames.AddRange(new string[] {
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"VaRest"
		});

		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.Add("DeveloperSettings");
		}

		DynamicallyLoadedModuleNames.AddRange(new string[] {});
	}
}
