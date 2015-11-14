// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "FitHUD.generated.h"



UENUM(BlueprintType)
enum class EGUIScreen : uint8
{
	ENone 					UMETA(DisplayName = "None"),
	EChoosePlayer	 		UMETA(DisplayName = "EChoosePlayer"),
	EWaitingPlayersScreen	UMETA(DisplayName = "EWaitingPlayersScreen"),
	ECountDownStartScreen	UMETA(DisplayName = "ECountDownStartScreen"),
	EPlayerInfoScreen		UMETA(DisplayName = "EPlayerInfoScreen"),
	EHostGameScreen			UMETA(DisplayName = "EHostGameScreen")
};


/**
 * 
 */
UCLASS()
class NETWORKRUNGAME_API AFitHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AFitHUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> WidgetMenuTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> PlayerInfoWidgetTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> AimWidgetTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> LevelLoadingTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> GameResultTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> WaitForStartTemplate;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> CountDownTemplate;

	

	UPROPERTY()
		UUserWidget* WidgetMenuInstance;

	UPROPERTY()
		UUserWidget* PlayerInfoWidgetInstance;

	UPROPERTY()
		UUserWidget* AimWidgetInstance;

	UPROPERTY()
		UUserWidget* LevelLoadingWidgetInstance;

	UPROPERTY()
		UUserWidget* WidgetGameResultInstance;

	UPROPERTY()
		UUserWidget* WaitForStartInstance;

	UPROPERTY()
		UUserWidget* CountDownInstance;

	UUserWidget* CurrentWidget = nullptr;


	void CreateWidgets();
	void ShowGameResult();
	void HideGameResult();
	void CreateMenuWidget();
	void CreatePlayerInfoWidget();
	void CreateAimWidget();
	void CreateLevelLoading();
	void FlipFlopMenu();
	void ShowMenu();
	void HideMenu();
	void ShowLevelLoading();
	void HideLevelLoading();
	void ShowPlayerInfo();


	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;


	void ShowWaitingFoStartScreen();
	void HideWaitingFoStartScreen();
	void ShowCountDownScreen();


	void ShowGUI(EGUIScreen Screen);


	
};
