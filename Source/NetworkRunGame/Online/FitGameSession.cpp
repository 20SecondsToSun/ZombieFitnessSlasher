// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "FitGameSession.h"
#include "Engine.h"

AFitGameSession::AFitGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	CreateCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted)),
	StartCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted))
{

}

bool AFitGameSession::HostSession(FName SessionName, bool bIsLAN, int32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			FOnlineSessionSettings Settings;
			Settings.bIsLANMatch = true;

			CreateCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);

			return Sessions->CreateSession(MaxNumPlayers, SessionName, Settings);
		}
	}
#if !UE_BUILD_SHIPPING
	else
	{
		// Hack workflow in development
		//OnCreatePresenceSessionComplete().Broadcast(GameSessionName, true);
		return true;
	}
#endif

	return false;
}

void AFitGameSession::OnCreateCompleted(FName SessionName, bool bWasSuccessful)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "OnCreateCompleted");
}

void AFitGameSession::OnStartCompleted(FName SessionName, bool bWasSuccessful)
{

}