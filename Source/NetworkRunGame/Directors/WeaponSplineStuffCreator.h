// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Directors/SplineStuffCreator.h"
#include "WeaponSplineStuffCreator.generated.h"

UCLASS()
class NETWORKRUNGAME_API AWeaponSplineStuffCreator : public ASplineStuffCreator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* Ammo;

private:
	virtual UClass* ChooseSpawningStuff(const TArray<UClass*>& StuffArray) override;
	virtual void SpawnRandom(UClass* SpawnClass, const FSpawnStuffData& Data);		
};
