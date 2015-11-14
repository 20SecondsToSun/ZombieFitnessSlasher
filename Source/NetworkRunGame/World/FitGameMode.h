// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "FitGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKRUNGAME_API AFitGameMode : public AGameMode
{
	GENERATED_BODY()

	AFitGameMode();
	
	/* Called once on every new player that enters the gamemode */
	//virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal /* = TEXT("") */);

	virtual void InitGameState();

	virtual void StartMatch();

	virtual void Tick(float DeltaSeconds) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const TSharedPtr<const FUniqueNetId>& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void StartNewPlayer(APlayerController*) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

public:
	UFUNCTION(BlueprintCallable, Category = "GamePlay")
		void RestartLevel();

private:
	TArray<APlayerStart*> PlayerStarts;
	TArray<TSubclassOf<APawn>> PlayerCharactersClassArray;

	FTimerHandle StartGameTimer, WaitingScreenTimer, CountDownTimer;
	int PlayersNum = 1;

	
	void IntPlayersHandler();
	void WaitingHandler(APlayerController* Controller);



	void StartCountdownTimerForStartGame();
	void CountdownHandler();

};
