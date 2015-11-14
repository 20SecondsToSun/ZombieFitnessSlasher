// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy/EnemyCharacter.h"
#include "Components/SplineComponent.h"
#include "Components/EnemyRoadSplineComponent.h"
#include "EnemyRunCharacter.generated.h"

UCLASS()
class NETWORKRUNGAME_API AEnemyRunCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
		
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	AEnemyRunCharacter();

	UFUNCTION(BlueprintCallable, Category = "EnemyBehaviour")
		virtual void TryToDamageHero(AActor* DamageActor) override;

private:
	UEnemyRoadSplineComponent* RoadSplineComponent;

};
