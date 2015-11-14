// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class NETWORKRUNGAME_API StaticHelper
{
public:
	static FHitResult GetLandscapeHitResult(UWorld* const world, const FVector& startVec, const FVector& endVec);
};
