// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Player/PickupStuff/PickupStuffBase.h"
#include "RunArrowStuff.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NETWORKRUNGAME_API ARunArrowStuff : public APickupStuffBase
{
	GENERATED_BODY()
	
	
public:
	ARunArrowStuff();
	virtual void BeginPlay();

private:
	UMaterial* LaserMaterial;
	UStaticMeshComponent* arrowMesh;
};
