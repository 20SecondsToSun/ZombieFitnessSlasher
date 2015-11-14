// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/PickupFactory.h"
#include "Directors/SplineBorderCreator.h"
#include "Directors/SplineStuffCreator.h"

class NETWORKRUNGAME_API WeaponFactory: public PickupFactory
{
public:
	WeaponFactory(ASplineStuffCreator* creator);
	virtual void SpawnRandom(UWorld* const world, const FPickupData& data) override;

private:
	ASplineStuffCreator* creator;
};
