// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "FitPlayerController.h"
#include "Engine.h"
#include "World/FitGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "World/FitGameState.h"
#include "Serial/USerialA.h"
#include "Enemy/EnemyCharacter.h"
#include "Player/FitPlayerState.h"
#include <string>

AFitPlayerController::AFitPlayerController() :RunState(NONE)
{

}

void AFitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);	
		
	InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AFitPlayerController::FlipFlopMenu);
}

void AFitPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (RightAuthorityMode())
	{
		GameSettings.Init();
	}	
}

void AFitPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (RightAuthorityMode())
	{
		CloseSerial();
	}	
}

void AFitPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);
	OpenSerial();
}

void AFitPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	ReadSerial();	
}

void AFitPlayerController::HostLobby()
{	
	float seconds = 1.0f;
	FTimerHandle _Timer;
	FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitPlayerController::HostLobbyTimeOut);		
	GetWorldTimerManager().SetTimer(_Timer, del, seconds, false);
}

void AFitPlayerController::HostLobbyTimeOut()
{	
	auto GameInst = Cast<UFitGameInstance>(GetGameInstance());	
	if (GameInst)
	{
		GameInst->HostLobby();
	}		
}

void AFitPlayerController::JoinLobby_Implementation()
{
//	HideMenu();
//	ShowLevelLoading();

	float seconds = 1.0f;
	FTimerHandle _Timer;
	FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitPlayerController::JoinLobbyTimeOut);
	GetWorldTimerManager().SetTimer(_Timer, del, seconds, false);
}

void AFitPlayerController::JoinLobbyTimeOut()
{
	auto GameInst = Cast<UFitGameInstance>(GetGameInstance());

	if (GameInst)
		GameInst->JoinLobby();
}

void AFitPlayerController::DestroyLobby()
{
	auto gameI = Cast<UFitGameInstance>(GetGameInstance());

	if (gameI)
		gameI->DestroyLobby();
}

bool AFitPlayerController::JoinLobby_Validate()
{
	return true;
}

void AFitPlayerController::OpenSerial_Implementation()
{	
	if (RightAuthorityMode())
	{
		if (!USerialBPF::IsSerialPortOpen())
		{
			USerialBPF::OpenSerial(GameSettings.ComPortNumberButtons, 9600);
			//trace(FColor::Green, "try open buttons-------------------------------  " + FString::FromInt(GameSettings.ComPortNumberButtons));
		}

		if (USerialBPF::IsSerialPortOpen())
		{
			SetArduinoResistance_Implementation(GameSettings.DefaultResistance, 2);
			//trace(FColor::Green, "set resistance -------------------------------  " + FString::FromInt(ResistanceMiddle));
		}			
		else
			GameSettings.ArduinoEnabled = false;

		if (!UUSerialA::IsSerialPortOpen())
		{
			UUSerialA::OpenSerial(GameSettings.ComPortNumberSpeed, 9600);
			//trace(FColor::Green, "try open arduino-------------------------------  " + FString::FromInt(GameSettings.ComPortNumberSpeed));
		}
	}	
}

void AFitPlayerController::ReadSerial_Implementation()
{
	if (RightAuthorityMode() && GameSettings.ArduinoEnabled && InputEnabled())
	{	
		if (USerialBPF::IsSerialPortOpen())
		{
			auto aCommand = USerialBPF::ReadSerial();
			DoArduinoButtonsCommands(aCommand);
		}

		if (UUSerialA::IsSerialPortOpen())
		{
			auto aCommand = UUSerialA::ReadSerial();
			DoArduinoSpeedCommands(aCommand);
		}
	}	
}

void AFitPlayerController::CloseSerial_Implementation()
{
	if (RightAuthorityMode() && GameSettings.ArduinoEnabled)
	{
		USerialBPF::CloseSerial();
		UUSerialA::CloseSerial();
		FString message = "------------------------------- CloseSerial -------------------------------";
		UE_LOG(LogClass, Log, TEXT("%s"), *message);		
	}
}

void AFitPlayerController::SetArduinoResistance_Implementation(int32 Resistance, float Time)
{
	if (RightAuthorityMode() && CurrentResistance != Resistance && GameSettings.ArduinoEnabled)
	{
		if (USerialBPF::IsSerialPortOpen())
		{
			auto AResistance = "preset " + FString::FromInt(Resistance) + "!";
			USerialBPF::WriteSerial(AResistance);
			trace(FColor::Green, "set resistance-------------------------------  " + AResistance);	
			
			
			CurrentResistance = Resistance;
			if (Time != -1)
			{
				GetWorldTimerManager().ClearTimer(ResistanceTimer);
				FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitPlayerController::ResistanceTimerHandler);
				GetWorldTimerManager().SetTimer(ResistanceTimer, del, Time, false);
			}
		}
		else
			trace(FColor::Red, "serial not open-------------------------------  ");
	}		
}

void AFitPlayerController::ResistanceTimerHandler()
{
	SetArduinoResistance(GameSettings.DefaultResistance, -1);
}

void AFitPlayerController::DoArduinoSpeedCommands(const FString& commanString)
{
	auto FitCharacter = Cast<AFitCharacter>(GetCharacter());

	if (!RightAuthorityMode() || !FitCharacter || !GameSettings.ArduinoEnabled)
		return;
	
	if (commanString.Len() > 0)
	{
		///trace(FColor::Blue, "commandString :::: " + commanString);

		TArray<FString> values;
		commanString.ParseIntoArray(values, TEXT("="));
		if (values.Num() > 1)
		{
			if (values[0] == "time")
			{
				auto speedValue = FCString::Atof(*values[1]);

				FColor color = FColor::Red;
				if (speedValue == -1)
					color = FColor::White;		

				auto speed = 0.0f;
				auto currentVelocity = 0.0f;

				if (speedValue != -1)
				{				
					auto InputRange = FVector2D(GameSettings.FastArduinoValue, GameSettings.SlowArduinoValue);
					auto OutputRange = FVector2D(FitCharacter->MAX_PLAYER_VELOCITY, 0.0f);

					speed = speedValue * 0.001f;
					currentVelocity = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, speed);
				}
				
				ArduinoVelocity = speed;
				
				FitCharacter->SetCurrentVelocity(currentVelocity);
				FitCharacter->CurrentVelocity = currentVelocity;									
			}
		}
	}
}

void AFitPlayerController::DoArduinoButtonsCommands(const FString& commanString)
{
	auto FitCharacter = Cast<AFitCharacter>(GetCharacter());

	if (!RightAuthorityMode() || !FitCharacter || !GameSettings.ArduinoEnabled)
		return;
	
	if (commanString.Len() > 0)
	{	
		TArray<FString> outCommands;
		commanString.ParseIntoArray(outCommands, TEXT(","));

		TMap<FString, FString> ControlCmdMap;

		for (auto command : outCommands)
		{
			TArray<FString> values;
			command.ParseIntoArray(values, TEXT("="));

			if (values.Num() > 1)
			{
				ControlCmdMap.Add(values[0]) = values[1];
			}
		}

	
		if (ControlCmdMap.Num() < 6)
			return;	

		if (ControlCmdMap[GameSettings.LEFT_CODE] == GameSettings.ON_CODE)
		{
			FitCharacter->OnLeft();
		}
		else if (ControlCmdMap[GameSettings.RIGHT_CODE] == GameSettings.ON_CODE)
		{
			FitCharacter->OnRight();
		}

		if (ControlCmdMap[GameSettings.FIRE_CODE] == GameSettings.ON_CODE)
		{
			FitCharacter->Fire();
		}
	}
}

void AFitPlayerController::SleepTimerHandler()
{
	GetWorldTimerManager().ClearTimer(SleepTimer);
	sleep = false;
}

void AFitPlayerController::SetCurrentRunStateTimer_Implementation()
{
	GetWorldTimerManager().ClearTimer(CurrentRunStateTimer);
	FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitPlayerController::CurrentRunStateTimerHandler);
	GetWorldTimerManager().SetTimer(CurrentRunStateTimer, del, GameSettings.MaxIdleSeconds, true);
}

void AFitPlayerController::CurrentRunStateTimerHandler()
{
	CAN_MOVE = false;

	auto FitCharacter = Cast<AFitCharacter>(GetCharacter());
	if (FitCharacter)
	{
		FitCharacter->SetCurrentVelocity(0.0f);
		FitCharacter->CurrentVelocity = 0.0f;
	}		
}

FString AFitPlayerController::GetCommandValue(const FString& command, const FString& CommandName)
{
	if (command.Find(CommandName) != -1)
	{
		TArray<FString> outCommand;
		command.ParseIntoArray(outCommand, TEXT("="));

		if (outCommand.Num() > 1)
			return outCommand[1];
	}

	return "0";
}

bool AFitPlayerController::CheckToStop()
{	
	float diff = GetWorld()->GetTimeSeconds() - LastNoneRunTime;

	if (RunState == RUNNING)
		LastNoneRunTime = GetWorld()->GetTimeSeconds();

	return  diff > GameSettings.MaxIdleSeconds;
}

void AFitPlayerController::CalculateCharacterVelocityFromArduinoData(AFitCharacter* FitCharacter)
{
	float aVel = GetWorld()->GetTimeSeconds();// FCString::Atof(*aVelocity) * 0.001f;
	auto value = aVel - LastArduinoVelocityValue;

	if (value < 0.1f)
		return;//// value = FastArduinoValue;

	if (value > GameSettings.SlowArduinoValue)
		value = GameSettings.SlowArduinoValue;

	LastArduinoVelocityValue = aVel;
	ArduinoVelocity = value;	

	auto InputRange = FVector2D(GameSettings.FastArduinoValue, GameSettings.SlowArduinoValue);
	auto OutputRange = FVector2D(FitCharacter->MAX_PLAYER_VELOCITY, 0.0f);
	auto currentVelocity = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, value);
	auto _FitCharacter = Cast<AFitCharacter>(GetCharacter());

	if (_FitCharacter)
	{
		_FitCharacter->SetCurrentVelocity(currentVelocity);
		_FitCharacter->CurrentVelocity = currentVelocity;
	}
}

bool AFitPlayerController::RightAuthorityMode() const
{
	return Role >= ROLE_AutonomousProxy;
}

void AFitPlayerController::trace(FColor DisplayColor, const FString& DebugMessage)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, DisplayColor, DebugMessage);
}

void AFitPlayerController::CameraFadeDamage_Implementation()
{	
	PlayerCameraManager->StartCameraFade(0.0f, 0.6f, 0.5f, FLinearColor::Red);
	SetArduinoResistance(GetMaxResistance(), 8.0f);	
}

void AFitPlayerController::GyperRun_Implementation()
{
	SetArduinoResistance(GetMinResistance(), 5.0f);
}

void AFitPlayerController::DisableAllInput_Implementation()
{
	//DisableInput(this);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "=========DisableAllInput=====");

	auto FitCharacter = Cast<AFitCharacter>(GetCharacter());
	if (FitCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "=========Disable FitCharacter=====");
		FitCharacter->DisableInput(this);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "========= Cannt Disable FitCharacter=====");
	}
}

void AFitPlayerController::EnableAllInput_Implementation()
{
	//EnableInput(this);

	auto FitCharacter = Cast<AFitCharacter>(GetCharacter());
	if (FitCharacter)
	{
		FitCharacter->EnableInput(this);
	}
}

void AFitPlayerController::FlipFlopMenu_Implementation()
{
	AFitHUD* MainHUD = Cast<AFitHUD>(GetHUD());
	if (MainHUD)
	{	
		MainHUD->FlipFlopMenu();
	}
}

void AFitPlayerController::ShowGUI_Implementation(EGUIScreen Screen)
{
	AFitHUD* MainHUD = Cast<AFitHUD>(GetHUD());
	if (MainHUD)
	{
		MainHUD->ShowGUI(Screen);
	}		
}
