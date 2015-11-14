// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "EnemyCharacter.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Player/FitCharacter.h"
#include "Player/FitPlayerController.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor> ImpactBlueprint(TEXT("/Game/game/FX/Blood_Explosion"));
	if (ImpactBlueprint.Class != NULL)
	{
		FXImpactClass = ImpactBlueprint.Class;
	}

	AimComponent = CreateDefaultSubobject<UAimComponent>(TEXT("AimComponent"));
	AimComponent->SetSceneLinkedComponent(GetRootComponent());
}

void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AEnemyCharacter::TryToDamageHero(AActor* DamageActor)
{
	if (IsAlive() && IsAttacking && DamageActor)
	{
		auto Character = Cast<AFitCharacter>(DamageActor);

		if (Character)
		{
			auto FitController = Cast<AFitPlayerController>(Character->GetController());
			if (FitController)
				FitController->CameraFadeDamage();
		}		
	}
}

void AEnemyCharacter::DamageApply_Implementation(const FVector& Location)
{
	Health--;

	if (Health <= 0)
	{
		_IsAlive = false;

		if (AimComponent)
		{
			AimComponent->IsAlive = false;
			//AimComponent->SetDefaultHighlight();
		}			
	}		

	auto Blood = GetWorld()->SpawnActor<AActor>(FXImpactClass, Location, FRotator::ZeroRotator);
	Blood->SetActorScale3D(FVector(4.0f, 4.0f, 4.0f));
}

bool AEnemyCharacter::IsAlive()
{
	return _IsAlive;
}

void AEnemyCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyCharacter, IsAttacking);
	DOREPLIFETIME(AEnemyCharacter, _IsAlive);
	DOREPLIFETIME(AEnemyCharacter, Health);
}
