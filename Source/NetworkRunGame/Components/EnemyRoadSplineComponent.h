// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/RoadSplineComponent.h"
#include "EnemyRoadSplineComponent.generated.h"

UCLASS()
class NETWORKRUNGAME_API UEnemyRoadSplineComponent : public URoadSplineComponent
{
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Reliable, Category = "PlayerColor")
	void SetActorRotationMulticast(const FRotator& rotator);
	void SetActorRotationMulticast_Implementation(const FRotator& rotator);
	bool SetActorRotationMulticast_Validate(const FRotator& rotator){ return true; };	

public:
	void InitStartPoint();
	virtual void MoveCharacterlongSplineTrack(ACharacter* Actor) override;

protected:
	virtual void SetNextPoint() override;	
};
