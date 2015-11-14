// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "RoadSplineComponent.h"
#include "Engine.h"

URoadSplineComponent::URoadSplineComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	bReplicates = true;
}

void URoadSplineComponent::BeginPlay()
{
	Super::BeginPlay();	

	auto World = GetWorld();

	if (World)
	{
		for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
		{
			RoadSpline = Cast<ARoadSpline>(*ActorItr);
			if (RoadSpline)
			{
				if (!RoadSpline->GetSplineComponent())
					RoadSpline->FindTrack();
				break;
			}
		}
	}
}

void URoadSplineComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void URoadSplineComponent::SetActiveTrackID(int32 value)
{
	ActiveTrackID = value;
	CurrentTrackNum = value;
}

void URoadSplineComponent::TurnToLeft()
{
	if (CurrentTrackNum - 1 > 0)
	{
		CurrentTrackNum -= 1;
		SetForwardPoint();
	}
}

void URoadSplineComponent::TurnToRight()
{
	if (CurrentTrackNum  < ARoadSpline::TRACKS_COUNT)
	{
		CurrentTrackNum += 1;
		SetForwardPoint();
	}
}

void URoadSplineComponent::SetForwardPoint()
{
	SetNextPoint();
	SetNextPoint();
}

void URoadSplineComponent::SetNextPoint()
{
	CurrentPointNum = (CurrentPointNum + 1) % RoadSpline->GetTrackPointsNum();
}

float URoadSplineComponent::GetRemainMeters()
{
	if (RoadSpline)
		return RoadSpline->GetTrackPointsNum() - CurrentPointNum;

	return 0;
}

void URoadSplineComponent::MoveCharacterlongSplineTrack(ACharacter* Actor)
{
	if (RoadSpline)
		RoadSpline->SetActorAtSplineTrackPoint(Actor , this);
}

void URoadSplineComponent::ResetProgress()
{
	CurrentDistancePointNum = 0;
}

int32 URoadSplineComponent::GetPlayerSplineDistancePoint() const
{
	return CurrentDistancePointNum;
}

int32 URoadSplineComponent::GetCurrentTrackNum() const
{
	return CurrentTrackNum;
}

int32 URoadSplineComponent::GetCurrentPointNum() const
{
	return CurrentPointNum;
}

bool URoadSplineComponent::IsNextLoop() const
{
	return false;// CurrentPointNum >= RoadSpline->GetTrackPointsNum() - 1;
}
