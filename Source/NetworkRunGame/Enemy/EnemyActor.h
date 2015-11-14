// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"
#include "EnemyActor.generated.h"

UCLASS()
class NETWORKRUNGAME_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Aim")
		UAimComponent* AimComponent;

	UFUNCTION(Server, Reliable, WithValidation, Category = "EnemyBehaviour")
		virtual void DamageApply(const FVector& Location);

	bool DamageApply_Validate(const FVector& Location) { return true; };
	void DamageApply_Implementation(const FVector& Location);

	bool IsAlive();

private:
	UClass* FXImpactClass;
};
