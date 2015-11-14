// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "FitGameMode.h"
#include "Engine.h"
#include "Player/FitPlayerState.h"
#include "Player/FitPlayerController.h"
#include "Player/FitCharacter.h"
#include "World/FitGameState.h"
#include "Tools/RoadSpline.h"
#include "Tools/Math/Math.h"
#include "World/FitGameState.h"
#include "Online/FitGameSession.h"
#include "UI/FitHUD.h"

AFitGameMode::AFitGameMode()
{
	FString CharacterNames[] =
	{
		"/Game/game/characters/characters/ThirdPersonCharacter",
		"/Game/game/characters/characters/ThirdPersonCharacter6",
		"/Game/game/characters/characters/ThirdPersonCharacter5",
		"/Game/game/characters/characters/ThirdPersonCharacter4",
		"/Game/game/characters/characters/ThirdPersonCharacter3",
		"/Game/game/characters/characters/ThirdPersonCharacter2",
		"/Game/game/characters/characters/ThirdPersonCharacter"
	};

	for (size_t i = 0; i < ARRAY_COUNT(CharacterNames); i++)
	{
		ConstructorHelpers::FClassFinder<APawn> PlayerCharacterBP(*CharacterNames[i]);
		if (PlayerCharacterBP.Class != NULL)
		{
			PlayerCharactersClassArray.Add(PlayerCharacterBP.Class);
		}
	}	

	DefaultPawnClass = nullptr;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/game/characters/characters/FitPlayerController_BP"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}	

	PlayerControllerClass = AFitPlayerController::StaticClass();
	PlayerStateClass = AFitPlayerState::StaticClass();	
	HUDClass = AFitHUD::StaticClass();
	GameStateClass = AFitGameState::StaticClass();	
}

void AFitGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString ConfigsPath = FPaths::GameUserDir();
	ConfigsPath += "RunTrainGame.ini";

	if (FPaths::FileExists(ConfigsPath))
	{
		FConfigSection* Configs = GConfig->GetSectionPrivate(TEXT("Section.Main"), false, true, ConfigsPath);

		if (Configs)
		{
			for (FConfigSection::TIterator It(*Configs); It; ++It)
			{			
				if (It.Key().GetPlainNameString() == "PlayersNum")
				{
					PlayersNum = FCString::Atoi(*It.Value());
				}					
			}
		}
	}
	else
	{
		// show message: ini fail doesn't exist
	}
}

void AFitGameMode::PreLogin(const FString& Options, const FString& Address, const TSharedPtr<const FUniqueNetId>& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AFitGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);		

	AFitPlayerController* Controller = Cast<AFitPlayerController>(NewPlayerController);

	if (Controller)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Yellow, "Can Start? ::   " + FString::FromInt(GetNumPlayers()) + "   " + FString::FromInt(PlayersNum - 1));		

		if (GetNumPlayers() > PlayersNum - 1)
		{
			FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitGameMode::IntPlayersHandler);
			GetWorldTimerManager().SetTimer(StartGameTimer, del, 2.0f, false);
		}
		else
		{
			//FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitGameMode::WaitingHandler, Controller);
			//GetWorldTimerManager().SetTimer(WaitingScreenTimer, del, 0.2f, false);
		}
	}
}

void AFitGameMode::WaitingHandler(APlayerController* Controller)
{
	//AFitHUD* MainHUD = Cast<AFitHUD>(Controller->GetHUD());

	//if (MainHUD)
	//	MainHUD->ShowGUI(EGUIScreen::EWaitingPlayersScreen);
}

void AFitGameMode::StartNewPlayer(APlayerController* _controller)
{
	Super::StartNewPlayer(_controller);

}

void AFitGameMode::IntPlayersHandler()
{	
	TArray<AFitCharacter*> characters;	
	auto World = GetWorld();

	if (World)
	{
		TArray<AActor*> PlayerStartsArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartsArray);

		for (size_t i = 0; i < PlayerStartsArray.Num(); i++)
		{
			auto PlayerStart = Cast<APlayerStart>(PlayerStartsArray[i]);
			if (PlayerStart)
			{
				PlayerStarts.Add(PlayerStart);
			}
		}

		TArray<AActor*> FitCharactersArray;
	
		for (size_t i = 0; i < GetNumPlayers(); i++)
		{
			auto Spawn = World->SpawnActor<AFitCharacter>(PlayerCharactersClassArray[i], PlayerStartsArray[i]->GetActorLocation(), FRotator::ZeroRotator);
			if (Spawn)
			{
				characters.Add(Spawn);
			}
				
		}
		
		/*UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFitCharacter::StaticClass(), FitCharactersArray);


		for (size_t i = 0; i < FitCharactersArray.Num(); i++)
		{
			auto pawn = Cast<AFitCharacter>(FitCharactersArray[i]);
			if (pawn)
			{
				characters.Add(pawn);
			}
		}*/		

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "characters:::  " + FString::FromInt(characters.Num()));

		int32 plStartIndex = 0;
		int i = 0;

		StartCountdownTimerForStartGame();

		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			AFitPlayerController* Controller = Cast<AFitPlayerController>(*Iterator);

			if (Controller)
			{		
				if (i < characters.Num())
				{
					Controller->Possess(characters[i]);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, "=========POSES == == == == =");
				}					
			
				//Controller->DisableAllInput_Implementation();
				Controller->DisableAllInput();
				Controller->ShowGUI(EGUIScreen::ECountDownStartScreen);

				auto character = Cast<AFitCharacter>(Controller->GetCharacter());
				if (character)
				{		
					//	character->SetPlayerColor(PlayerColors[i]);
					character->SetTrackID(i + 1);

					if (PlayerStarts.Num())
					{
						character->SetActorLocation(PlayerStarts[plStartIndex]->GetActorLocation());
						character->SetActorRotation(PlayerStarts[plStartIndex]->GetActorRotation());
						plStartIndex = (plStartIndex + 1) % PlayerStarts.Num();
					}
				}
			}
			++i;
		}
	}	
}

void AFitGameMode::StartCountdownTimerForStartGame()
{	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "============   timer start  ======================");
	auto _Gamestate = Cast<AFitGameState>(GameState);

	if (_Gamestate)
	{
		_Gamestate->InitReadyForStartCounter();

		FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitGameMode::CountdownHandler);
		GetWorldTimerManager().SetTimer(CountDownTimer, del, 1.0f, true);
	}	
}

void AFitGameMode::CountdownHandler()
{
	auto gamestate = Cast<AFitGameState>(GameState);

	if (gamestate)
	{
		gamestate->DecreaseReadyForStartCounter();

		if (gamestate->IsReadyForStartCountercomplete())
		{
			GetWorldTimerManager().ClearTimer(CountDownTimer);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "============   timer complete  ======================");			

			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				AFitPlayerController* Controller = Cast<AFitPlayerController>(*Iterator);
				if (Controller)
				{
					Controller->ShowGUI(EGUIScreen::EPlayerInfoScreen);				
					Controller->EnableAllInput();
				}		
			}			
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "Count for start::  " + FString::FromInt(gamestate->GetCurrentCountDown()));
		}			
	}	
}

void AFitGameMode::RestartLevel()
{
	int32 plStartIndex = 0;

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), )

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFitPlayerController* controller = Cast<AFitPlayerController>(*Iterator);
		if (controller)
		{
			//controller->HideGameResult();
			
			auto Character = Cast<AFitCharacter>(controller->GetCharacter());
			if (Character)
			{
				if (PlayerStarts.Num())
				{
					Character->SetActorLocation(PlayerStarts[plStartIndex]->GetActorLocation());
					Character->SetActorRotation(PlayerStarts[plStartIndex]->GetActorRotation());
					Character->ResetParams();
					Character->DistanceOnTrack = 0;
					plStartIndex = (plStartIndex + 1) % PlayerStarts.Num();
				}
			}
		}
	}

	FString LEVEL_NAME = "RunGameLevelEmpty";
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LEVEL_NAME), true, "listen");
}

void AFitGameMode::InitGameState()
{
	Super::InitGameState();	
}

void AFitGameMode::StartMatch()
{
	//if (GetNumPlayers() >= 2)
	Super::StartMatch();
}

void AFitGameMode::Tick(float DeltaSeconds)
{
	TArray<AFitCharacter*> characters;

	int i = 0;
	bool IsGameFinished = false;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFitPlayerController* controller = Cast<AFitPlayerController>(*Iterator);
		if (controller)
		{
			auto character = Cast<AFitCharacter>(controller->GetCharacter());		
			if (character)
			{
				characters.Add(character);
				IsGameFinished = character->IsWin();
				if (IsGameFinished)
					break;
			}
		}		
	}


	if (IsGameFinished)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			AFitPlayerController* controller = Cast<AFitPlayerController>(*Iterator);
			//if (controller)
			//	controller->ShowGameResult();
		}

		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			AFitPlayerController* controller = Cast<AFitPlayerController>(*Iterator);
			if (controller)
			{
				controller->SetPause(true);
				controller->bEnableClickEvents = true;
				controller->bShowMouseCursor = true;
			}
				

		}
	}

	//UE_LOG(LogClass, Log, TEXT("--------------------"));

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "char len  " + FString::FromInt(characters.Num()));


	auto _GameState = Cast<AFitGameState>(GameState);

	if (_GameState)
	{
		_GameState->Characters = characters;
	}
	
}