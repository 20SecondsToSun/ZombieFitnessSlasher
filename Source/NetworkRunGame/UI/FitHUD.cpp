// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "FitHUD.h"

AFitHUD::AFitHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetBPClass(TEXT("/Game/game/Interface/Menu_Widget"));
	if (MenuWidgetBPClass.Succeeded())
	{
		WidgetMenuTemplate = MenuWidgetBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerInfoBPClass(TEXT("/Game/game/Interface/PlayerInfo_Widget"));
	if (PlayerInfoBPClass.Succeeded())
	{
		PlayerInfoWidgetTemplate = PlayerInfoBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> AimBPClass(TEXT("/Game/game/Interface/Aim_Widget"));
	if (AimBPClass.Succeeded())
	{
		AimWidgetTemplate = AimBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LevelLoadingBPClass(TEXT("/Game/game/Interface/LevelLoading"));
	if (LevelLoadingBPClass.Succeeded())
	{
		LevelLoadingTemplate = LevelLoadingBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> GameResultBPClass(TEXT("/Game/game/Interface/GameResult_Widget"));
	if (GameResultBPClass.Succeeded())
	{
		GameResultTemplate = GameResultBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WaitForStartBP(TEXT("/Game/game/Interface/WaitForStart"));
	if (WaitForStartBP.Succeeded())
	{
		WaitForStartTemplate = WaitForStartBP.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> CountDownBP(TEXT("/Game/game/Interface/CountDown"));
	if (CountDownBP.Succeeded())
	{
		CountDownTemplate = CountDownBP.Class;
	}
}

void AFitHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateWidgets();
	//ShowPlayerInfo();
	ShowGUI(EGUIScreen::EWaitingPlayersScreen);
}

void AFitHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFitHUD::CreateWidgets()
{
	CreatePlayerInfoWidget();
	CreateAimWidget();	
	CreateLevelLoading();

	if (WidgetMenuTemplate && !WidgetGameResultInstance)
	{
		WidgetGameResultInstance = CreateWidget<UUserWidget>(GetWorld(), GameResultTemplate);
	}

	if (WaitForStartTemplate && !WaitForStartInstance)
	{
		WaitForStartInstance = CreateWidget<UUserWidget>(GetWorld(), WaitForStartTemplate);
		WaitForStartInstance->AddToViewport();
		WaitForStartInstance->SetVisibility(ESlateVisibility::Hidden);		
	}

	if (CountDownTemplate && !CountDownInstance)
	{
		CountDownInstance = CreateWidget<UUserWidget>(GetWorld(), CountDownTemplate);
		CountDownInstance->AddToViewport();
		CountDownInstance->SetVisibility(ESlateVisibility::Hidden);
	}

	CreateMenuWidget();	
}

void AFitHUD::ShowGameResult()
{
	if (WidgetGameResultInstance && !WidgetGameResultInstance->IsInViewport())
	{
		WidgetGameResultInstance->AddToViewport();
		WidgetGameResultInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void AFitHUD::HideGameResult()
{
	if (WidgetGameResultInstance)
	{
		WidgetGameResultInstance->RemoveFromViewport();
	}
}

void AFitHUD::CreateMenuWidget()
{
	if (WidgetMenuTemplate && !WidgetMenuInstance)
	{
		WidgetMenuInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetMenuTemplate);
		if (WidgetMenuInstance)
		{
			WidgetMenuInstance->AddToViewport();
			HideMenu();
		}
	}
}

void AFitHUD::CreatePlayerInfoWidget()
{
	if (PlayerInfoWidgetTemplate && !PlayerInfoWidgetInstance)
	{
		PlayerInfoWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PlayerInfoWidgetTemplate);
		if (PlayerInfoWidgetInstance)
		{
			PlayerInfoWidgetInstance->AddToViewport();
			PlayerInfoWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AFitHUD::CreateAimWidget()
{
	if (AimWidgetTemplate && !AimWidgetInstance)
	{
		AimWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), AimWidgetTemplate);
		if (AimWidgetInstance)
		{
			AimWidgetInstance->AddToViewport();
			AimWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AFitHUD::CreateLevelLoading()
{
	if (LevelLoadingTemplate && !LevelLoadingWidgetInstance)
	{
		LevelLoadingWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LevelLoadingTemplate);
		if (LevelLoadingWidgetInstance)
		{
			LevelLoadingWidgetInstance->AddToViewport();
			LevelLoadingWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AFitHUD::FlipFlopMenu()
{
	if (WidgetMenuInstance)
		WidgetMenuInstance->GetVisibility() == ESlateVisibility::Visible ? HideMenu() : ShowMenu();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "FlipFlopMenu !!!");
}

void AFitHUD::ShowMenu()
{
	WidgetMenuInstance->SetVisibility(ESlateVisibility::Visible);
	FInputModeGameAndUI Mode;
	//SetInputMode(Mode);
	//bShowMouseCursor = true;
}

void AFitHUD::HideMenu()
{
	WidgetMenuInstance->SetVisibility(ESlateVisibility::Hidden);

	FInputModeGameOnly Mode;
	//SetInputMode(Mode);
	//bShowMouseCursor = false;
}

void AFitHUD::ShowLevelLoading()
{
	LevelLoadingWidgetInstance->SetVisibility(ESlateVisibility::Visible);
}

void AFitHUD::HideLevelLoading()
{
	LevelLoadingWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}

void AFitHUD::ShowPlayerInfo()
{
	if (PlayerInfoWidgetInstance)
	{
		PlayerInfoWidgetInstance->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void AFitHUD::ShowWaitingFoStartScreen()
{
	//if (WaitForStartInstance)
	//	WaitForStartInstance->SetVisibility(ESlateVisibility::Visible);
}

void AFitHUD::HideWaitingFoStartScreen()
{
	//if (WaitForStartInstance)
	//	WaitForStartInstance->SetVisibility(ESlateVisibility::Hidden);
}

void AFitHUD::ShowCountDownScreen()
{

}


void AFitHUD::ShowGUI(EGUIScreen Screen)
{
	if (CurrentWidget)
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	

	switch (Screen)
	{
		case EGUIScreen::EChoosePlayer:
		
			break;

		case EGUIScreen::EWaitingPlayersScreen:
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Yellow, "EWaitingPlayersScreen ::   ");
			CurrentWidget = WaitForStartInstance;
			break;

		case EGUIScreen::ECountDownStartScreen:
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Yellow, "ECountDownStartScreen ::   ");
			CurrentWidget = CountDownInstance;
			break;

		case EGUIScreen::EPlayerInfoScreen:
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Yellow, "EPlayerInfoScreen ::   ");
			CurrentWidget = PlayerInfoWidgetInstance;
			break;
			
	}

	if (CurrentWidget)
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
}

