// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Player/Weapon/WeaponEnum.h"
#include "Directors/SplineStuffCreator.h"
#include "Tools/FitTypes.h"
#include "Directors/SplineBorderCreator.h"
#include "PickupStuffBase.generated.h"

UCLASS()
class NETWORKRUNGAME_API APickupStuffBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupStuffBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		void OnComponentBeginOverlapHandler(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetRespawner(ASplineStuffCreator* creator);
	void SetPickupData(const FPickupData& data);

protected:
	float distance;
	int32 roadNum;
	int32 wCount;
	bool IsAlive = true;

	WeaponEnum::WeaponType wType;
	
	UPrimitiveComponent* meshComp;
	ASplineStuffCreator* creator;

	EGraphicInterfaceEnum GrInterfaceEnum;
	
};
