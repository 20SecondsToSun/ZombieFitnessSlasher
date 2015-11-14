// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Components/SplineComponent.h"
#include "SplineBorderCreator.generated.h"

USTRUCT()
struct NETWORKRUNGAME_API FPickupData
{
	GENERATED_USTRUCT_BODY()

	FPickupData(const FVector& position, const FRotator& rotation = FRotator::ZeroRotator, float distance = 0.0f, int32 roadNum = 1, float scale = 1.0f)
	{
		this->position = position;
		this->rotation = rotation;
		this->distance = distance;
		this->roadNum = roadNum;
		this->scale = scale;
	}

	FPickupData(){};

	FVector position;
	FRotator rotation;

	float distance;
	float roadNum;
	float scale;
};

UCLASS()
class NETWORKRUNGAME_API ASplineBorderCreator : public AActor
{
	GENERATED_BODY()
	
	struct TrackPoint
	{
		FVector directionAtPoint;
		FVector worldLocation;
	};

	TArray<TrackPoint> TrackPoints;
	UClass* BuiTrackBall;

public:
	// Sets default values for this actor's properties
	ASplineBorderCreator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeightUnderTrack = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StepBetween = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Spline;	

private:
	USplineComponent* currentTrack;
	enum TrackBorderType
	{
		LEFT,
		RIGHT
	};

	void CreateBorderOfBalls();
	FVector GetTrackPointAtPointNumAndTrack(int PointNum, TrackBorderType TrackType) const;
};
