// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "BigBangGunProjectile.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Player/FitCharacter.h"
#include "Player/Weapon/Guns/BigBangGun.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/EnemyActor.h"

// Sets default values
ABigBangGunProjectile::ABigBangGunProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshlAsset(TEXT("/Game/game/Weapon/Meshes/SM_ExampleMesh_Rocket"));
	if (MeshlAsset.Succeeded())
	{
		MissleSkeletalMeshComp->SetStaticMesh(MeshlAsset.Object);
	}
}
