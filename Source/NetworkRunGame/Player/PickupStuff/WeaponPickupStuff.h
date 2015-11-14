// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Player/PickupStuff/PickupStuffBase.h"
#include "WeaponPickupStuff.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKRUNGAME_API AWeaponPickupStuff : public APickupStuffBase
{
	GENERATED_BODY()
	
public:	
	AWeaponPickupStuff();	

	virtual void BeginPlay() override;


};
