// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Player/Weapon//Guns/IWeapon.h"
#include "Components/ArrowComponent.h"
#include "BigBangGun.generated.h"

UCLASS()
class NETWORKRUNGAME_API ABigBangGun : public AIWeapon
{
	GENERATED_BODY()
	
public:	
	ABigBangGun();
};
