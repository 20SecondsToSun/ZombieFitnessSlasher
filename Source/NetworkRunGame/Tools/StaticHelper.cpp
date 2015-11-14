// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "StaticHelper.h"

FHitResult StaticHelper::GetLandscapeHitResult(UWorld* const World, const FVector& startVec, const FVector& endVec)
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("LanscapeTrace")), true);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	

	FHitResult Hit(ForceInit);
	TArray<FHitResult> Hits;

	World->LineTraceSingleByObjectType(Hit, startVec, endVec, ECollisionChannel::ECC_GameTraceChannel1, TraceParams);
	return Hit;
}
