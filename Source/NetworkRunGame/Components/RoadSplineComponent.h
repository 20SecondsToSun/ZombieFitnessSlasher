// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Tools/RoadSpline.h"
#include "RoadSplineComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKRUNGAME_API URoadSplineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URoadSplineComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void TurnToLeft();
	void TurnToRight();
	float GetRemainMeters();

	virtual void MoveCharacterlongSplineTrack(ACharacter* Actor);

	void SetActiveTrackID(int32 TrackID);
	void ResetProgress();

	int32 GetPlayerSplineDistancePoint() const;
	int32 GetCurrentTrackNum() const;
	int32 GetCurrentPointNum() const;
	void SetForwardPoint();
	bool IsNextLoop() const;

protected:
	class ARoadSpline* RoadSpline;
	int32 ActiveTrackID = 1;	
	int32 CurrentTrackNum = 3;
	int32 CurrentPointNum = 1;	
	int32 CurrentDistancePointNum = 0;	
	virtual void SetNextPoint();	
};
