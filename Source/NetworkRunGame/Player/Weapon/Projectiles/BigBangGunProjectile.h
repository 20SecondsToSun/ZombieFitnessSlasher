// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Player/Weapon/Projectiles/IProjectile.h"
#include "GameFramework/Actor.h"

#include "BigBangGunProjectile.generated.h"

UCLASS()
class NETWORKRUNGAME_API ABigBangGunProjectile : public AIProjectile
{
	GENERATED_BODY()
	
public:	
	ABigBangGunProjectile();

};
