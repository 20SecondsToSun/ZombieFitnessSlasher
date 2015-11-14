// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "WeaponSplineStuffCreator.h"
#include "Engine.h"
#include "Player/Weapon/Guns/IWeapon.h"

UClass* AWeaponSplineStuffCreator::ChooseSpawningStuff(const TArray<UClass*>& StuffArray)
{
	if (FMath::FRand() > 0.6f)
	{
		if (StuffArray.Num() > 0)
		{
			auto index = FMath::RandRange(0, StuffArray.Num() - 1);
			return StuffArray[index];
		}
	}

	return Ammo;	
}

void AWeaponSplineStuffCreator::SpawnRandom(UClass* SpawnClass, const FSpawnStuffData& Data)
{
	auto World = GetWorld();
	
	if (World)
	{
		auto Spawn = World->SpawnActor<AActor>(SpawnClass, Data.Position, Data.Rotation);

		if (Spawn && SpawnClass)
		{
			auto Scale = Data.Scale;

			if (SpawnClass == Ammo)
			{
				Scale = 7;
			}

			Spawn->SetActorScale3D(FVector(Scale, Scale, Scale));
		}
	}		
}
