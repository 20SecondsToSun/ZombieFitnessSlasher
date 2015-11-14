// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "FitPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"
#include "Player/FitCharacter.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/EnemyActor.h"

void AFitPlayerState::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFitPlayerState, Player_ID);
	DOREPLIFETIME(AFitPlayerState, TrackID);
	DOREPLIFETIME(AFitPlayerState, PlayerColor);	
	DOREPLIFETIME(AFitPlayerState, EnemyAimStruct);
	DOREPLIFETIME(AFitPlayerState, PrevEnemyAimStruct);
}

void AFitPlayerState::SetAim_Implementation(FEnemyAimStruct EnemyAim)
{
	PrevEnemyAimStruct = EnemyAimStruct;
	EnemyAimStruct = EnemyAim;	
}

void AFitPlayerState::SetHighLightsON_Implementation()
{
	if (EnemyAimStruct.AnemyAim)
	{
		EnemyAimStruct.AnemyAim->SetColorHighlight();
	}
}

void AFitPlayerState::SetHighLightsOFF_Implementation()
{
	if (EnemyAimStruct.AnemyAim)
	{
		EnemyAimStruct.AnemyAim->SetDefaultHighlight();
	}			
}

bool AFitPlayerState::IsAimClose()
{
	return  EnemyAimStruct.AnemyAim != nullptr;
}

bool AFitPlayerState::IfAimEquals(FEnemyAimStruct EnemyAim)
{
	return EnemyAimStruct.AnemyAim == EnemyAim.AnemyAim;
}

void AFitPlayerState::SetEnemyAimOnFire()
{
	if (EnemyAimStruct.AnemyAim)
	{
		EnemyAimStruct.AnemyAim->SetEnemyAimOnFire();
		EnemyAimStruct.AnemyAim->SetDefaultHighlight_Implementation();
	}
}

UAimComponent* AFitPlayerState::GetAnemyAim()
{
	return EnemyAimStruct.AnemyAim;
}
