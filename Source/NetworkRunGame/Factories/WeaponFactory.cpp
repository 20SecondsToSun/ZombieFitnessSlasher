// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "WeaponFactory.h"
#include "Player/PickupStuff/WeaponPickupStuff.h"
#include "Player/PickupStuff/FreezePickupStuff.h"
#include "Player/PickupStuff/RunArrowStuff.h"
#include "Directors/SplineBorderCreator.h"

WeaponFactory::WeaponFactory(ASplineStuffCreator* creator)
{
	this->creator = creator;
}

void WeaponFactory::SpawnRandom(UWorld* const world, const FPickupData& data)
{
	if (world)
	{
		APickupStuffBase* spawn = nullptr;/// = world->SpawnActor<AWeaponPickupStuff>(position, rotation);

		auto val = FMath::FRand();

		//if (val <= 0.7f)
		//{
		spawn = world->SpawnActor<AWeaponPickupStuff>(data.position, data.rotation);
		//}			
		//else// if (val > 0.3f && val <= 0.8f)
		//{
			//spawn = world->SpawnActor<ARunArrowStuff>(data.position - FVector(0.0, 0.0, 100.0), data.rotation);			
		//}
		//else
		//{
		//	spawn = world->SpawnActor<AFreezePickupStuff>(data.position, data.rotation);
		//}			

		if (spawn)
		{
			spawn->SetActorScale3D(FVector(data.scale, data.scale, data.scale));
			spawn->SetPickupData(data);
			spawn->SetRespawner(creator);
		}			
	}
}
