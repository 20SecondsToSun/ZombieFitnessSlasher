// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "SplineBorderCreator.h"
#include "Tools/RoadSpline.h"
#include "Tools/StaticHelper.h"

ASplineBorderCreator::ASplineBorderCreator()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;
	
	static ConstructorHelpers::FClassFinder<AActor> ItemBlueprint(TEXT("/Game/game/blueprints/Bui_BP"));
	if (ItemBlueprint.Succeeded())
	{
		BuiTrackBall = ItemBlueprint.Class;
	}
}

void ASplineBorderCreator::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		currentTrack = nullptr;
		auto RSpline = Cast<ARoadSpline>(Spline);

		if (RSpline)
		{			
			currentTrack = RSpline->GetSplineComponent();
			if (currentTrack)
			{
				float DistanceAtTrack = 0.0f;

				while (DistanceAtTrack < currentTrack->GetSplineLength())
				{
					TrackPoint Point;
					Point.directionAtPoint = currentTrack->GetWorldDirectionAtDistanceAlongSpline(DistanceAtTrack);
					Point.worldLocation = currentTrack->GetWorldLocationAtDistanceAlongSpline(DistanceAtTrack);

					TrackPoints.Add(Point);
					DistanceAtTrack += StepBetween;
				}

				CreateBorderOfBalls();
			}
		}			
	}	
}

void ASplineBorderCreator::CreateBorderOfBalls()
{
	auto World = GetWorld();

	if (BuiTrackBall && World)
	{
		auto GenerateBallPosition = [&](int i, TrackBorderType trackNum)
		{
			auto position = GetTrackPointAtPointNumAndTrack(i, trackNum);
			auto topPoint = position;
			auto bottomPoint = position;

			topPoint.Z = 100000;
			bottomPoint.Z = -100000;

			auto placeBallPosition = StaticHelper::GetLandscapeHitResult(World, topPoint, bottomPoint).ImpactPoint;
			placeBallPosition.Z += HeightUnderTrack;

			return placeBallPosition;
		};

		for (size_t i = 0; i < TrackPoints.Num(); i++)
		{
			World->SpawnActor<AActor>(BuiTrackBall, GenerateBallPosition(i, TrackBorderType::LEFT), FRotator::ZeroRotator);
			World->SpawnActor<AActor>(BuiTrackBall, GenerateBallPosition(i, TrackBorderType::RIGHT), FRotator::ZeroRotator);
		}
	}
}

FVector ASplineBorderCreator::GetTrackPointAtPointNumAndTrack(int PointNum, TrackBorderType TrackType) const
{
	auto location = TrackPoints[PointNum].worldLocation;
	auto direction = TrackPoints[PointNum].directionAtPoint;
	direction = direction.RotateAngleAxis(90, FVector::UpVector);

	float BallPosition = 0.0f;
	float BorderShift = 200.0f;

	switch (TrackType)
	{
	case TrackBorderType::LEFT:
		BallPosition = 2 * ARoadSpline::TRACKS_OFFSET + BorderShift;
		break;

	case TrackBorderType::RIGHT:
		BallPosition = -2 * ARoadSpline::TRACKS_OFFSET - BorderShift;
		break;
	}

	return location + direction * BallPosition;
}
