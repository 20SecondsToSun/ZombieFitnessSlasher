// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Online.h"
#include "Engine.h"
#include "GameFramework/GameSession.h"
#include "FitGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKRUNGAME_API UFitGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFitGameInstance(const FObjectInitializer& ObjectInitializer);

	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void HostLobby();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void JoinLobby();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void DestroyLobby();

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SolusHUDCrosshair)
	int32 InterLevelPersistentValue;

	UFUNCTION(BlueprintImplementableEvent, Category = "SessionEvent")
	void HostLobbyEvent(APlayerController* host);

	UFUNCTION(BlueprintImplementableEvent, Category = "SessionEvent")
	void JoinLobbyEvent(APlayerController* host);

	UFUNCTION(BlueprintImplementableEvent, Category = "SessionEvent")
	void LoadLevelEvent(APlayerController* host, int32 level);


	FDelegateHandle CreateCompleteDelegateHandle;
	FDelegateHandle StartCompleteDelegateHandle;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;


	FOnCreateSessionCompleteDelegate CreateCompleteDelegate;
	FOnStartSessionCompleteDelegate StartCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindAndJoinSessionsCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindAndDestroySessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;


	void OnCreateCompleted(FName SessionName, bool bWasSuccessful);
	void OnStartCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleted(bool bWasSuccessful);
	void OnFindAndJoinSessionCompleted(bool bWasSuccessful);
	void OnFindAndDestroySessionCompleted(bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence, FOnFindSessionsCompleteDelegate FindSessionDelegate);
	bool _JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	void DestroySession(FName SessionName);

private:	
	int32 PLAYERS_NUM = 3;
	bool IS_LAN = true;

	FString GAME_SESSION_PREFIX = "New Session";
	FString GameSessionName;
	FString LEVEL_NAME = "OnlineTestLevel";

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	void OpenLevelHandler();	
};
