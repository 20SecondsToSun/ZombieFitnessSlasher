// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "PickupStuffBase.h"
#include "Player/FitCharacter.h"
#include "Engine.h"
#include "Directors/SplineBorderCreator.h"

// Sets default values
APickupStuffBase::APickupStuffBase() :IsAlive(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void APickupStuffBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (meshComp && !meshComp->OnComponentBeginOverlap.IsAlreadyBound(this, &APickupStuffBase::OnComponentBeginOverlapHandler))
		{
			meshComp->OnComponentBeginOverlap.AddDynamic(this, &APickupStuffBase::OnComponentBeginOverlapHandler);
		}
	}
}

// Called every frame
void APickupStuffBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void APickupStuffBase::OnComponentBeginOverlapHandler(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
}

void APickupStuffBase::SetRespawner(ASplineStuffCreator* creator)
{
	this->creator = creator;
}

void APickupStuffBase::SetPickupData(const FPickupData& data)
{
	this->distance = data.distance;
	this->roadNum = data.roadNum;
}
