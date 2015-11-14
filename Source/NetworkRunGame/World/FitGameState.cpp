// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "FitGameState.h"
#include "Net/UnrealNetwork.h"


void AFitGameState::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFitGameState, Characters);
	DOREPLIFETIME(AFitGameState, CurrentCountDown);
}

void AFitGameState::InitReadyForStartCounter()
{
	CurrentCountDown = MAX_COUNT;
}

void AFitGameState::DecreaseReadyForStartCounter()
{
	CurrentCountDown -=1 ;
}

bool AFitGameState::IsReadyForStartCountercomplete()
{
	return CurrentCountDown == -1;
}

int32 AFitGameState::GetCurrentCountDown()
{
	return CurrentCountDown;
}