// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Directors/SplineStuffCreator.h"
#include "SpeedStuffCreator.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKRUNGAME_API ASpeedStuffCreator : public ASplineStuffCreator
{
	GENERATED_BODY()
	
	
private:
	virtual UClass* ChooseSpawningStuff(const TArray<UClass*>& StuffArray) override;
	virtual void SpawnRandom(UClass* SpawnClass, const FSpawnStuffData& Data);
	
};
