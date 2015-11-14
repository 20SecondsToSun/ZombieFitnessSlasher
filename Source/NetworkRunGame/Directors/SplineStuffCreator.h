// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tools/RoadSpline.h"
#include "SplineStuffCreator.generated.h"

USTRUCT()
struct NETWORKRUNGAME_API FSpawnStuffData
{
	GENERATED_USTRUCT_BODY()

	FSpawnStuffData()
	{

	}

	FSpawnStuffData(const FVector& Position, const FRotator& Rotator, float Scale, float Distance, float RoadNum)
	{
		this->Position = Position;
		this->Rotation = Rotator;
		this->Scale = Scale;
		this->Distance = Distance;
		this->RoadNum = RoadNum;
	}

	FVector Position;
	FRotator Rotation;
	float Scale;

	float Distance;
	float RoadNum;	
};

UCLASS()
class NETWORKRUNGAME_API ASplineStuffCreator : public AActor
{
	GENERATED_BODY()
	
public:	
	ASplineStuffCreator();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeightUnderTrack = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DistanceBetweenWeapons = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartAtDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UClass*> StuffArray;

	void Create();	
	void CreateOne(float distance, int32 RoadNumBusy);
	void _CreateOne(float distance, ARoadSpline* RSpline, int32 RoadNumBusy);

	virtual UClass* ChooseSpawningStuff(const TArray<UClass*>& StuffArray);
	virtual void SpawnRandom(UClass* SpawnClass, const FSpawnStuffData&);

private:
	FHitResult getTrackHitResult(UWorld* const world, const FVector& startVec, const FVector& endVec);	
};
