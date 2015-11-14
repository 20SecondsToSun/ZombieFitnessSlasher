// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Serial/SerialBPF.h"
#include "Player/FitCharacter.h"
#include "UI/FitHUD.h"
#include "FitPlayerController.generated.h"

USTRUCT()
struct NETWORKRUNGAME_API FGameSettings
{
	GENERATED_USTRUCT_BODY()
	
	int ComPortNumberButtons = 7;
	int ComPortNumberSpeed = 4;

	float BaudRate = 9600;
	float FastArduinoValue = 0.3;
	float SlowArduinoValue = 1.5;
	float MaxIdleSeconds = 1.6;
	bool ArduinoEnabled = false;
	bool IsInit = false;

	int32 MinResistance = 0;
	int32 MaxResistance = 13;
	int32 DefaultResistance = 5;

	FString ON_CODE;// = "1";
	FString OFF_CODE;// = "0";

	FString LEFT_CODE;// = "lm=";
	FString RIGHT_CODE;// = "rm=";

	FString FIRE_CODE;// = "lfm=";
	FString SPEED_CODE;// = "rfm=";

	void Init()
	{
		FString ConfigsPath = FPaths::GameUserDir();
		ConfigsPath += "RunTrainGame.ini";

		if (FPaths::FileExists(ConfigsPath) && !IsInit)
		{
			FConfigSection* Configs = GConfig->GetSectionPrivate(TEXT("Section.Arduino"), false, true, ConfigsPath);

			if (Configs)
			{
				for (FConfigSection::TIterator It(*Configs); It; ++It)
				{
					FString KeyString = It.Key().GetPlainNameString();
					FString ValueString = It.Value();

					if (KeyString == "ComPortNumberButtons")
					{
						ComPortNumberButtons = FCString::Atoi(*ValueString);
					}
					else if (KeyString == "ComPortNumberSpeed")
					{
						ComPortNumberSpeed = FCString::Atoi(*ValueString);
					}
					else if (KeyString == "ResistanceSlow")
					{
						MaxResistance = FCString::Atoi(*ValueString);
					}
					else if (KeyString == "ResistanceFast")
					{
						MinResistance = FCString::Atoi(*ValueString);
					}
					else if (KeyString == "ResistanceMiddle")
					{
						DefaultResistance = FCString::Atoi(*ValueString);
					}
					else if (KeyString == "Enabled")
					{
						ArduinoEnabled = (ValueString == "true");
					}
					else if (KeyString == "BaudRate")
					{
						BaudRate = FCString::Atof(*ValueString);
					}
					else if (KeyString == "FastArduinoValue")
					{
						FastArduinoValue = FCString::Atof(*ValueString);
					}
					else if (KeyString == "SlowArduinoValue")
					{
						SlowArduinoValue = FCString::Atof(*ValueString);
					}
					else if (KeyString == "MaxIdleSeconds")
					{
						MaxIdleSeconds = FCString::Atof(*ValueString);
					}
					else if (KeyString == "ON_CODE")
					{
						ON_CODE = ValueString;
					}
					else if (KeyString == "OFF_CODE")
					{
						OFF_CODE = ValueString;
					}
					else if (KeyString == "LEFT_CODE")
					{
						LEFT_CODE = ValueString;
					}
					else if (KeyString == "RIGHT_CODE")
					{
						RIGHT_CODE = ValueString;
					}
					else if (KeyString == "FIRE_CODE")
					{
						FIRE_CODE = ValueString;
					}
					else if (KeyString == "SPEED_CODE")
					{
						SPEED_CODE = ValueString;
					}
				}

				FConfigSection* MainConfigs = GConfig->GetSectionPrivate(TEXT("Section.Main"), false, true, ConfigsPath);

				if (MainConfigs)
				{
					UGameUserSettings* MyGameSettings = GEngine->GetGameUserSettings();
					MyGameSettings->SetVSyncEnabled(true);
					//MyGameSettings->set

					for (FConfigSection::TIterator It(*MainConfigs); It; ++It)
					{
						FString KeyString = It.Key().GetPlainNameString();
						FString ValueString = It.Value();

						if (KeyString == "Fullscreen")
						{
							ValueString == "ON" ? MyGameSettings->SetFullscreenMode(EWindowMode::Fullscreen) : MyGameSettings->SetFullscreenMode(EWindowMode::Windowed);
						}
						else  if (KeyString == "Resolution")
						{
							const FIntPoint DefaultResolutions[] = { FIntPoint(1024, 768), FIntPoint(1280, 720), FIntPoint(1920, 1080) };
							auto ResolutionID = FCString::Atoi(*ValueString);
							//MyGameSettings->SetScreenResolution(DefaultResolutions[ResolutionID]);
						}
						else  if (KeyString == "ScreenLog")
						{
							if (GEngine)
								GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, "ScreenLog  " + ValueString);
							if (GEngine)
							{
								GEngine->bEnableOnScreenDebugMessages = (ValueString == "ON");
							}							
						}						
					}

					MyGameSettings->ApplySettings(true);
					IsInit = true;
				}

			}
			else
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Yellow, "file doesnt exist");
			}
		}
	}
};

/**
 *
 */
UCLASS()
class NETWORKRUNGAME_API AFitPlayerController : public APlayerController
{
	GENERATED_BODY()

	AFitPlayerController();

public:
	bool IsCameraFollow = false;
	bool FollowSpline = false;


	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void SetPlayer(UPlayer* InPlayer);
	
	UFUNCTION(BlueprintCallable, Category = "Online")
		void HostLobby();

	//UFUNCTION(BlueprintCallable, Category = "Online")
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Online")
		void JoinLobby();

	UFUNCTION(BlueprintCallable, Category = "Online")
		void DestroyLobby();


	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Arduino")
		void OpenSerial();

	UFUNCTION(BlueprintCallable, Category = "Arduino")
		void DoArduinoSpeedCommands(const FString& commanString);

	UFUNCTION(BlueprintCallable, Category = "Arduino")
		void DoArduinoButtonsCommands(const FString& commanString);


	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Arduino")
		void SetArduinoResistance(int32 value, float time);

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Arduino")
		void ReadSerial();

	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Arduino")
		void CloseSerial();


	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "EnemyAction")
		void CameraFadeDamage();

	void CameraFadeDamage_Implementation();



	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	//	TSubclassOf<UUserWidget> WidgetMenuTemplate;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	//	TSubclassOf<UUserWidget> PlayerInfoWidgetTemplate;	

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	//	TSubclassOf<UUserWidget> AimWidgetTemplate;	

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	//	TSubclassOf<UUserWidget> LevelLoadingTemplate;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	//	TSubclassOf<UUserWidget> GameResultTemplate;

	//UPROPERTY()
	//	UUserWidget* WidgetMenuInstance;

	//UPROPERTY()
	//	UUserWidget* PlayerInfoWidgetInstance;

	//UPROPERTY()
	//	UUserWidget* AimWidgetInstance;

	//UPROPERTY()
	//	UUserWidget* LevelLoadingWidgetInstance;

	//UPROPERTY()
	//	UUserWidget* WidgetGameResultInstance;


	//UFUNCTION(Client, Reliable)
	//	void ShowGameResult();

	//UFUNCTION(Client, Reliable)
	//	void HideGameResult();



	//UFUNCTION(Client, Reliable)
	//	void CreateMenuWidgetOnClient();

	//UFUNCTION(Client, Reliable)
	//	void CreatePlayerInfoWidgetOnClient();

	//UFUNCTION(Client, Reliable)
	//	void CreateAimWidgetOnClient();

	//UFUNCTION(Client, Reliable)
	//	void CreateLevelLoadingOnClient();

	//UFUNCTION(Client, Reliable)
	//	void CreateWidgetsOnClient();	



	//UFUNCTION(Client, Reliable)
	//	void ShowPlayerInfo();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector2D ScreenLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float ArduinoVelocity = 0.0f;	
	

	UFUNCTION(Server, Withvalidation, Reliable)
		void SetCurrentRunStateTimer();

	void SetCurrentRunStateTimer_Implementation();
	bool SetCurrentRunStateTimer_Validate() {return true; };

	//void HideMenu();
	///void ShowLevelLoading();
	//void HideLevelLoading();

	void HostLobbyTimeOut();
	void JoinLobbyTimeOut();	

	int32 GetMinResistance() { return GameSettings.MinResistance; };
	int32 GetMaxResistance() { return GameSettings.MaxResistance; };
	int32 GetDefaultResistance() { return GameSettings.DefaultResistance; };

	UFUNCTION(Server, WithValidation, Reliable)
		void GyperRun();

	void GyperRun_Implementation();
	bool GyperRun_Validate(){ return true; };

	void CalculateCharacterVelocityFromArduinoData(AFitCharacter* FitCharacter);

	//void DisableAllInput();
	//void EnableAllInput();
	//void FlipFlopMenu();

	UFUNCTION(Client, Reliable)
		void ShowGUI(EGUIScreen Screen);

	UFUNCTION(Client, Reliable)
		void EnableAllInput();

	UFUNCTION(Client, Reliable)
		void DisableAllInput();

	UFUNCTION(Client, Reliable)
		void FlipFlopMenu();
	
private:
	bool CAN_MOVE = true;	

	FTimerHandle CurrentRunStateTimer;
	void CurrentRunStateTimerHandler();

	bool sleep = false;
	FTimerHandle SleepTimer;
	void SleepTimerHandler();

	void trace(FColor DisplayColor, const FString& DebugMessage);
	//void FlipFlopMenu();
	//void ShowMenu();
	
	void DelayHandler();
	bool CheckToStop();

	void OpenSerial_Implementation();
	void ReadSerial_Implementation();
	void Close_Implementation();	
	void SetArduinoResistance_Implementation(int32 resistance, float time);

	void ResistanceTimerHandler();
	
	FString GetCommandValue(const FString& command, const FString& CommandName);
	bool RightAuthorityMode() const;

	FTimerHandle ResistanceTimer;
	FString* ArduinoData;	

	enum Running
	{
		NONE,
		RUNNING
	}
	RunState;

	float LastNoneRunTime = 0.0f;
	float LastArduinoVelocityValue = 0.0f;	
	int32 CurrentResistance = -1;

	FGameSettings GameSettings;
	
};
