// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "Engine.h"
#include "EnemyRunCharacter.h"
#include "Tools/RoadSpline.h"
#include "Player/FitPlayerController.h"
#include "Net/UnrealNetwork.h"

AEnemyRunCharacter::AEnemyRunCharacter()
{
	RoadSplineComponent = CreateDefaultSubobject<UEnemyRoadSplineComponent>(TEXT("RoadSplineComponent"));
}

void AEnemyRunCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		RoadSplineComponent->InitStartPoint();
	}
}

void AEnemyRunCharacter::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		IsAlive() 
			? RoadSplineComponent->MoveCharacterlongSplineTrack(this) 
			: GetMovementComponent()->StopActiveMovement();	
	}	
}

void AEnemyRunCharacter::TryToDamageHero(AActor* DamageActor)
{
	if (IsAlive())
	{
		auto FitCharacter = Cast<AFitCharacter>(DamageActor);

		if (FitCharacter)
		{
			auto FitCharacterController = Cast<AFitPlayerController>(FitCharacter->GetController());

			if (FitCharacterController)
				FitCharacterController->CameraFadeDamage();
		}
	}
}
