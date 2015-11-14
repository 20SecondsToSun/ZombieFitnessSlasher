// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "WeaponPickupStuff.h"
#include "Engine.h"
#include "Player/FitCharacter.h"

// Sets default values
AWeaponPickupStuff::AWeaponPickupStuff()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	//RootComponent = meshComp;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalAsset(TEXT("/Game/game/pawns/enemies/FPWeapon/Mesh/SK_FPGun"));
	//if (SkeletalAsset.Succeeded())
	//{
	//	Cast<USkeletalMeshComponent>(meshComp)->SetSkeletalMesh(SkeletalAsset.Object);
	//}

	//meshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//meshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//meshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	////WeaponSkeletalMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

	//meshComp->bGenerateOverlapEvents = true;
	//SetActorEnableCollision(true);

	wType = WeaponEnum::WeaponType::GUN;
	GrInterfaceEnum = EGraphicInterfaceEnum::VE_GUN;
	wCount = 2;
}

void AWeaponPickupStuff::BeginPlay()
{
	Super::BeginPlay();

	if (meshComp)
		meshComp->SetWorldScale3D(FVector(2.2f, 2.2f, 2.2f));
}
