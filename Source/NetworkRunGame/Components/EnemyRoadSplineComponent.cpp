// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "EnemyRoadSplineComponent.h"

void UEnemyRoadSplineComponent::InitStartPoint()
{
	auto Owner = GetOwner();

	if (Owner && RoadSpline)
	{
		float MinDist = 5000;
		auto TrackPoints = RoadSpline->GetTrackPoints();

		for (size_t i = 0; i < TrackPoints.Num(); i++)
		{
			auto Dist = FVector::Dist(TrackPoints[i].WorldLocation, Owner->GetActorLocation());

			if (MinDist > Dist)
			{
				MinDist = Dist;
				CurrentPointNum = i;
			}
		}

		CurrentTrackNum = FMath::RandRange(0, ARoadSpline::TRACKS_COUNT);
	}	
}

void UEnemyRoadSplineComponent::MoveCharacterlongSplineTrack(ACharacter* Actor)
{
	if (RoadSpline)
	{
		FVector DirectionToRun = RoadSpline->CalDirectionToRun(Actor, this);
		DirectionToRun.Normalize();
		DirectionToRun.Z = 0;

		Actor->AddMovementInput(DirectionToRun, 10);
		SetActorRotationMulticast(DirectionToRun.Rotation());
	}
}

void UEnemyRoadSplineComponent::SetActorRotationMulticast_Implementation(const FRotator& rotator)
{
	auto Owner = GetOwner();

	if (Owner)
	{
		Owner->SetActorRotation(rotator);
	}
}

void UEnemyRoadSplineComponent::SetNextPoint()
{
	CurrentPointNum = (CurrentPointNum - 1) > 0 ? CurrentPointNum - 1 : RoadSpline->GetTrackPointsNum() - 1;
}
