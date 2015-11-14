// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "SpeedStuffCreator.h"


UClass* ASpeedStuffCreator::ChooseSpawningStuff(const TArray<UClass*>& StuffArray)
{	
	if (StuffArray.Num() > 0 && FMath::FRand() > 0.5f)
	{
		auto index = FMath::RandRange(0, StuffArray.Num() - 1);
		return StuffArray[index];
	}

	return nullptr;
}

void ASpeedStuffCreator::SpawnRandom(UClass* SpawnClass, const FSpawnStuffData& Data)
{
	auto World = GetWorld();

	if (World && SpawnClass)
	{		
		auto Spawn = World->SpawnActor<AActor>(SpawnClass, Data.Position, Data.Rotation);

		if (Spawn)
		{
			auto Scale = 0.8F;
			Spawn->SetActorScale3D(FVector(Scale, Scale, Scale));			
		}
	}
}
