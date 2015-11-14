// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "EnemyActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = StaticMeshComponent;

	AimComponent = CreateDefaultSubobject<UAimComponent>(TEXT("AimComponent"));
	AimComponent->SetSceneLinkedComponent(GetRootComponent());

	static ConstructorHelpers::FClassFinder<AActor> ImpactBlueprint(TEXT("/Game/game/FX/FX_Explosion"));
	if (ImpactBlueprint.Class != NULL)
	{
		FXImpactClass = ImpactBlueprint.Class;
	}	
}

void AEnemyActor::DamageApply_Implementation(const FVector& Location)
{
	auto World = GetWorld();
	if (World)
	{
		auto Impact = World->SpawnActor<AActor>(FXImpactClass, Location, FRotator::ZeroRotator);
		if (Impact)
			Impact->SetActorScale3D(FVector(4.0f, 4.0f, 4.0f));

		AimComponent->IsAlive = false;
		Destroy();
	}	
}

bool AEnemyActor::IsAlive()
{
	return AimComponent && AimComponent->IsAlive;
}
