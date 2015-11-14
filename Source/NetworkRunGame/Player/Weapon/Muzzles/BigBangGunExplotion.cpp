// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "BigBangGunExplotion.h"

// Sets default values
ABigBangGunExplotion::ABigBangGunExplotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABigBangGunExplotion::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ABigBangGunExplotion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}
