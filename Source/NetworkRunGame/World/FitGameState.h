// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "Player/FitCharacter.h"
#include "FitGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKRUNGAME_API AFitGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<AFitCharacter*> Characters;
	
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;

	const int32 MAX_COUNT = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 CurrentCountDown = MAX_COUNT;

	void InitReadyForStartCounter();

	void DecreaseReadyForStartCounter();

	bool IsReadyForStartCountercomplete();
	int32 GetCurrentCountDown();
};
