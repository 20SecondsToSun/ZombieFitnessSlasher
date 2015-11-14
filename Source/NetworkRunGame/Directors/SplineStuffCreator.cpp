// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "SplineStuffCreator.h"
#include "Factories/WeaponFactory.h"
#include "Engine.h"
#include "Tools/StaticHelper.h"

ASplineStuffCreator::ASplineStuffCreator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASplineStuffCreator::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Create();	
	}	
}

void ASplineStuffCreator::Create()
{
	if (Spline)
	{	
		float Distance = StartAtDistance;
		auto RSpline = Cast<ARoadSpline>(Spline);

		if (RSpline)
		{
			while (!RSpline->EndOfSpline(Distance))
			{
				_CreateOne(Distance, RSpline, -1);
				Distance += DistanceBetweenWeapons;
			}
		}
	}
}

void ASplineStuffCreator::_CreateOne(float Distance, ARoadSpline* RSpline, int32 RoadNumBusy = -1)
{
	int32 RoadNum = 1;

	if (RoadNumBusy == -1)
	{
		RoadNum = RSpline->GetRandomTrackNumber();
	}		
	else
	{
		RoadNum = RSpline->GetRandomTrackNumberExclude(RoadNumBusy);
	}		

	FVector Position = RSpline->GetPositionOnRoadAtDistance(Distance, RoadNum);
	FRotator Rotator = RSpline->GetWorldRotationAtDistanceAlongSpline(Distance);

	auto topPoint = Position;
	auto bottomPoint = Position;
	topPoint.Z = 100000;
	bottomPoint.Z = -100000;

	Position = StaticHelper::GetLandscapeHitResult(GetWorld(), topPoint, bottomPoint).ImpactPoint;
	Position.Z += HeightUnderTrack;

	auto SpawnClass = ChooseSpawningStuff(StuffArray);
	FSpawnStuffData Data(Position, Rotator, FMath::FRandRange(1.5f, 2.0f), Distance, RoadNum);
	SpawnRandom(SpawnClass, Data);
}

void ASplineStuffCreator::CreateOne(float distance, int32 RoadNumBusy)
{
	if (HasAuthority() && Spline)
	{
		auto RSpline = Cast<ARoadSpline>(Spline);
		if (RSpline)
		{
			_CreateOne(distance, RSpline, RoadNumBusy);
		}
	}	
}

UClass* ASplineStuffCreator::ChooseSpawningStuff(const TArray<UClass*>& StuffArray)
{
	if (StuffArray.Num() > 0)
		return StuffArray[0];

	return nullptr;
}

void ASplineStuffCreator::SpawnRandom(UClass* SpawnClass, const FSpawnStuffData& Data)
{

}