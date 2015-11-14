// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "RoadSpline.h"
#include "Engine.h"
#include "Tools/Math/Math.h"
#include "Player/FitCharacter.h"
#include "Tools/StaticHelper.h"

int ARoadSpline::TRACKS_COUNT = 5;
float ARoadSpline::TRACKS_OFFSET = 460.0f;
float ARoadSpline::STEP = 500.0f;

ARoadSpline::ARoadSpline()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARoadSpline::BeginPlay()
{
	Super::BeginPlay();
	FindTrack();
}

USplineComponent* ARoadSpline::GetSplineComponent()
{
	return Track;
}

void ARoadSpline::FindTrack()
{
	Track = nullptr;
	TArray<UActorComponent*> Components;
	GetComponents(Components);

	for (auto Component : Components)
	{
		Track = Cast<USplineComponent>(Component);
		if (Track)
		{
			break;
		}	
	}
	
	if (Track)
	{
		float DistanceAtTrack = 0.0f;

		while (DistanceAtTrack < Track->GetSplineLength())
		{
			FTrackPoint Point;
			Point.DirectionAtPoint = Track->GetWorldDirectionAtDistanceAlongSpline(DistanceAtTrack);
			Point.WorldLocation = Track->GetWorldLocationAtDistanceAlongSpline(DistanceAtTrack);

			trackPoints.Add(Point);
			DistanceAtTrack += STEP;
		}
	}
}

bool ARoadSpline::EndOfSpline(float position) const
{
	return Track && position >= Track->GetSplineLength();
}

int32 ARoadSpline::GetRandomTrackNumber() const
{
	return FMath::RandRange(1, TRACKS_COUNT);
}

int32 ARoadSpline::GetRandomTrackNumberExclude(int32 BusyTrackNum) const
{
	TArray<int32> RoadsIndexes;

	for (size_t i = 1; i < TRACKS_COUNT + 1; i++)
	{
		if (BusyTrackNum != i)
			RoadsIndexes.Add(i);
	}

	auto index = FMath::RandRange(0, RoadsIndexes.Num()-1);
	return RoadsIndexes[index];
}

FVector ARoadSpline::GetPositionOnRoadAtDistance(float distance, int32 roadNum)
{
	if (!Track)	return FVector::ZeroVector;

	FVector splineVec = Track->GetWorldLocationAtDistanceAlongSpline(distance);
	FVector direction = Track->GetWorldDirectionAtDistanceAlongSpline(distance);
	direction = direction.RotateAngleAxis(90, FVector::UpVector);
	float value = 0.0f;

	switch (roadNum)
	{
		case 1:	value = -2 * TRACKS_OFFSET;	break;
		case 2:	value = -TRACKS_OFFSET;	break;
		case 3:	value = 0;	break;
		case 4:	value = TRACKS_OFFSET; break;
		case 5:	value = 2 * TRACKS_OFFSET;	break;
	}

	return splineVec + direction * value;
}

FVector ARoadSpline::GetDirectionOnRoadAtDistance(float distance)
{
	return Track ? Track->GetWorldDirectionAtDistanceAlongSpline(distance) : FVector::ZeroVector;
}

FRotator ARoadSpline::GetWorldRotationAtDistanceAlongSpline(float Distance) const
{
	return Track ? Track->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World) : FRotator::ZeroRotator;
}

bool ARoadSpline::HasNextSteeringPoint() const
{
	return Track && trackPoints.Num() > 0;
}

void ARoadSpline::SetActorAtSplineTrackPoint(ACharacter* Character, URoadSplineComponent* RoadSplineComponent)
{
	if (Track && HasNextSteeringPoint())
	{
		FVector directionToRun = CalDirectionToRun(Character, RoadSplineComponent);
		directionToRun.Normalize();		
		directionToRun.Z = Character->GetActorLocation().Z;

		auto player = Cast<AFitCharacter>(Character);		
		if (player)
		{		
			player->AddCustomMovementInput(directionToRun);					
		}	
	}
}

FVector ARoadSpline::CalDirectionToRun(ACharacter* Character, URoadSplineComponent* RoadSplineComponent)
{
	auto splinePtPos = GetSteeringPoint(RoadSplineComponent);
	auto splinePtPosXY = FVector2D(splinePtPos.X, splinePtPos.Y);
	auto actorLocXY = FVector2D(Character->GetActorLocation().X, Character->GetActorLocation().Y);
	float const Distance = FVector2D::Distance(splinePtPosXY, actorLocXY);

	 if (Distance < 300)
			RoadSplineComponent->SetForwardPoint();

	return splinePtPos - Character->GetActorLocation();
}

FVector ARoadSpline::GetSteeringPoint(URoadSplineComponent* RoadSplineComponent)
{	
	auto GenerateBallPosition = [&](int i, int trackNum)
	{
		auto position = GetTrackPointAtPointNumAndTrack(i, trackNum);
		auto topPoint = position;
		auto bottomPoint = position;

		topPoint.Z = 100000;
		bottomPoint.Z = -100000;

		auto placeBallPosition = StaticHelper::GetLandscapeHitResult(GetWorld(), topPoint, bottomPoint).ImpactPoint;
		placeBallPosition.Z += 200; 

		return placeBallPosition;
	};

	return GenerateBallPosition(RoadSplineComponent->GetCurrentPointNum(), RoadSplineComponent->GetCurrentTrackNum());
}

FVector ARoadSpline::GetTrackPointAtPointNumAndTrack(int pointNum, int TrackNum) const
{
	auto location  = trackPoints[pointNum].WorldLocation;
	auto direction = trackPoints[pointNum].DirectionAtPoint;
	direction = direction.RotateAngleAxis(90, FVector::UpVector);

	float value = 0.0f;

	switch (TrackNum)
	{
		case 1:	value = -2 * TRACKS_OFFSET;	break;
		case 2:	value = -TRACKS_OFFSET;		break;
		case 3:	value = 0.0f;	break;
		case 4:	value = TRACKS_OFFSET; break;
		case 5:	value = 2 * TRACKS_OFFSET; break;
	}

	return location + direction * value;
}

int32 ARoadSpline::GetTrackPointsNum() const
{
	return trackPoints.Num();
}

TArray<FTrackPoint> ARoadSpline::GetTrackPoints() const
{
	return trackPoints;
}
