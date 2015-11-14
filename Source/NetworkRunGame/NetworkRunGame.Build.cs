// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetworkRunGame : ModuleRules
{
	public NetworkRunGame(TargetInfo Target)
	{
        //PublicDependencyModuleNames.AddRange(
        //    new string[] { "Core",
        //        "CoreUObject",
        //        "Engine", 
        //        "InputCore",
        //        "UMG",
        //        "Slate",
        //        "SlateCore" ,              
        //        "OnlineSubsystemNull"});



        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",  "UMG",
                "Slate",
                "SlateCore" ,   "OnlineSubsystem", "OnlineSubsystemUtils" , "OnlineSubsystemNull"});
        PublicDependencyModuleNames.AddRange(new string[] { "AIModule" });
       // DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

	}
}
