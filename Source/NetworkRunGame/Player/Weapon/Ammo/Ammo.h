// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UCLASS()
class NETWORKRUNGAME_API AAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmo();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* AmmoMeshComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PatronsNum = 1;
	
};
