// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameSession.h"
#include "Online.h"
#include "FitGameSession.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKRUNGAME_API AFitGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	AFitGameSession(const FObjectInitializer& ObjectInitializer);
	bool HostSession(FName SessionName, bool bIsLAN, int32 MaxNumPlayers);
	
private:
	// Handles to the registered delegates above
	FDelegateHandle CreateCompleteDelegateHandle;
	FDelegateHandle StartCompleteDelegateHandle;

	// The delegate executed by the online subsystem
	FOnCreateSessionCompleteDelegate CreateCompleteDelegate;

	// The delegate executed by the online subsystem
	FOnStartSessionCompleteDelegate StartCompleteDelegate;

	// Internal callback when session creation completes, calls StartSession
	void OnCreateCompleted(FName SessionName, bool bWasSuccessful);

	// Internal callback when session creation completes, calls StartSession
	void OnStartCompleted(FName SessionName, bool bWasSuccessful);

};
