// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
//#include "Components/RoadSplineComponent.h"
#include "RoadSpline.generated.h"

USTRUCT()
struct NETWORKRUNGAME_API FTrackPoint
{
	GENERATED_USTRUCT_BODY()

	FTrackPoint(const FVector& DirectionAtPoint, const FVector& WorldLocation)
	{
		this->DirectionAtPoint = DirectionAtPoint;
		this->WorldLocation = WorldLocation;
	}

	FTrackPoint(){};

	FVector DirectionAtPoint;
	FVector WorldLocation;
};

UCLASS()
class NETWORKRUNGAME_API ARoadSpline : public AActor
{
	GENERATED_BODY()

public:
	static int TRACKS_COUNT;
	static float TRACKS_OFFSET;
	static float STEP;

	ARoadSpline();

	//virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	USplineComponent* GetSplineComponent();

	bool EndOfSpline(float position) const;
	int32 GetRandomTrackNumber() const;
	int32 GetRandomTrackNumberExclude(int32 BusyTrackNum) const;

	FVector GetPositionOnRoadAtDistance(float distance, int32 roadNum);
	FVector GetDirectionOnRoadAtDistance(float distance);
	FRotator GetWorldRotationAtDistanceAlongSpline(float distance) const;

	void SetActorAtSplineTrackPoint(ACharacter* Actor , class URoadSplineComponent* RoadSplineComponent);

	int GetPlayerSplineIndex() const;
	int32 GetTrackPointsNum() const;
	TArray<FTrackPoint> GetTrackPoints() const;
	FVector CalDirectionToRun(ACharacter* character, class URoadSplineComponent* RoadSplineComponent);

	
	void FindTrack();

private:	
	FVector GetSteeringPoint(class URoadSplineComponent* RoadSplineComponent);
	FVector GetTrackPointAtPointNumAndTrack(int pointNum, int trackNum) const;

	
	void setForwardPoint();
	void setNextPoint();
	bool HasNextSteeringPoint() const;

	USplineComponent* Track;
	TArray<FTrackPoint> trackPoints;
};
