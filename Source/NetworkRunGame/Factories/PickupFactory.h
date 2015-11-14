// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Directors/SplineBorderCreator.h"

class NETWORKRUNGAME_API PickupFactory
{
public:	
	virtual ~PickupFactory();
	virtual void SpawnRandom(UWorld* const world, const FPickupData& data) = 0;
};
