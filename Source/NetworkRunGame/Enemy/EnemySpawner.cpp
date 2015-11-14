// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxColllisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColllisionComponent"));
	RootComponent = BoxColllisionComponent;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();	
}

void AEnemySpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}
