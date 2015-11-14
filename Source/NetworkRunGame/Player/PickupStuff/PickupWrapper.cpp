// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "PickupWrapper.h"


// Sets default values
APickupWrapper::APickupWrapper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupWrapper::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupWrapper::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

