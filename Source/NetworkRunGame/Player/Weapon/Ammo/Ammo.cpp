// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "Ammo.h"


// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	AmmoMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMeshComp"));

	
	FString AmmoMeshPath = "";

	//static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> AmmoMeshAsset(*AmmoMeshPath);
	//if (AmmoMeshAsset.Succeeded())
	//{
		//AmmoMeshComp->set(AmmoMeshAsset.Object);
	//}
//
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();


	
	
}

// Called every frame
void AAmmo::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

