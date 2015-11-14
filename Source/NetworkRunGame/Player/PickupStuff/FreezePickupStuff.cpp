// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "FreezePickupStuff.h"
#include "Engine.h"
#include "Player/FitCharacter.h"

// Sets default values
AFreezePickupStuff::AFreezePickupStuff()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphComp"));
	RootComponent = meshComp;
	//Cast<USphereComponent>(meshComp)->InitSphereRadius(40.0f);
	//meshComp->SetProfileName(TEXT("SphereComponent"));

	auto fireParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	fireParticleSystem->AttachTo(meshComp);
	fireParticleSystem->bAutoActivate = true;
	fireParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	fireParticleSystem->SetActive(true);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
		fireParticleSystem->SetTemplate(ParticleAsset.Object);

	wType = WeaponEnum::WeaponType::FREEZE;
	GrInterfaceEnum = EGraphicInterfaceEnum::VE_FREEZE;
	wCount = 2;

	SetActorEnableCollision(true);
}
