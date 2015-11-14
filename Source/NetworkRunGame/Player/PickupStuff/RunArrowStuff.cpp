// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "RunArrowStuff.h"
#include "Player/FitCharacter.h"
#include "Player/FitPlayerController.h"
#include "Engine.h"

// Sets default values
ARunArrowStuff::ARunArrowStuff()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	arrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowAsset(TEXT("/Game/game/blueprints/Run_Mesh"));
	if (ArrowAsset.Succeeded())
	{
		Cast<UStaticMeshComponent>(arrowMesh)->SetStaticMesh(ArrowAsset.Object);
		arrowMesh->SetRelativeRotation(FRotator(0, 90, 0));
		arrowMesh->SetWorldScale3D(FVector(0.3, 0.3, 0.3));
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> LaserAsset(TEXT("/Game/game/materials/RunArrow_Material"));
	if (LaserAsset.Succeeded())
	{
		LaserMaterial = LaserAsset.Object;	
	}

	meshComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));	
	meshComp->SetCollisionProfileName(TEXT("boxComp"));
	meshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	meshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	meshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	meshComp->bGenerateOverlapEvents = true;	

	arrowMesh->AttachTo(meshComp);
	RootComponent = meshComp;

	wType = WeaponEnum::WeaponType::FREEZE;
	wCount = 2;
	
	SetActorEnableCollision(true);
}

void ARunArrowStuff::BeginPlay()
{
	Super::BeginPlay();

	if (meshComp)
		meshComp->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));

	if (LaserMaterial)
	{
		UMaterialInstanceDynamic* dynMaterial = UMaterialInstanceDynamic::Create(LaserMaterial, this);
		dynMaterial->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Green);
		dynMaterial->SetScalarParameterValue(TEXT("GlowIntensity"), 10.0f);
		arrowMesh->SetMaterial(0, dynMaterial);
	}
}