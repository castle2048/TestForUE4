// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class TestBlank : ModuleRules
{
    private string ModulePath {
        get
        {
            return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty"));
        }
    }

	public TestBlank(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
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
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicIncludePaths.Add(Path.Combine(ModulePath, "Hello", "Include"));
            PublicLibraryPaths.Add(Path.Combine(ModulePath, "Hello", "Lib"));
            PublicAdditionalLibraries.Add("HelloDll2.lib");

            Definitions.AddRange(new string[] { "LIB_EXPORT_API" });
            //PublicDelayLoadDLLs.Add("HelloDll2.dll");
        }else if(Target.Platform == UnrealTargetPlatform.Android)
        {
            //  TODO Utils.MakePathRelativeTo() UE4Win引擎内没有找到这个函数 怀疑是Android环境配套的API
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "TestBlank_APL.xml"));
        }
    }
}
