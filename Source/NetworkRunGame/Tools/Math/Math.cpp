// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "Math.h"

float Math::angleBetweenVectors(const FVector& v1, const FVector& v2)
{
	auto dotp = FVector::DotProduct(v1, v2);
	auto radians = FMath::Acos(dotp);
	auto degrees = FMath::RadiansToDegrees(radians);
	return degrees;
}

FRotator Math::RotBetweenVect(const FVector& A, const FVector& B)
{
	FRotator DeltaRot;
	//FVector  ATop, BTop;   //Top projections of the vectors
	//FVector  ASide, BSide; //Side projections of the vectors

	//ATop = A;
	//BTop = B;
	//ATop.Z = 0;
	//BTop.Z = 0;

	//ASide = A;
	//BSide = B;
	//ASide.Y = 0;
	//BSide.Y = 0;

	//DeltaRot.Yaw = acos(Normal(ATop) dot Normal(BTop)) * RadToUnrRot;
	//DeltaRot.Pitch = acos(Normal(ASide) dot Normal(BSide)) * RadToUnrRot;
	//DeltaRot.Roll = 0;

	return DeltaRot;
}

float Math::CalculateAngleBetweenActorsDeg(AActor* CHaracterActor, AActor* AimActor)
{
	auto CharacterNormForward = CHaracterActor->GetActorForwardVector();
	CharacterNormForward.Normalize();

	auto TargetLocation = AimActor->GetActorLocation();

	auto VecvtorToTarget = TargetLocation - CHaracterActor->GetActorLocation();
	VecvtorToTarget.Normalize();

	auto frwdVec = CHaracterActor->GetActorLocation() + 500 * CharacterNormForward;
	auto toTrgtVec = CHaracterActor->GetActorLocation() + 500 * VecvtorToTarget;
	toTrgtVec.Z = frwdVec.Z = 0;

	frwdVec = CharacterNormForward;
	toTrgtVec = VecvtorToTarget;

	auto dot = toTrgtVec.X * frwdVec.X + toTrgtVec.Y * frwdVec.Y;//  # dot product
	auto det = toTrgtVec.X * frwdVec.Y - toTrgtVec.Y * frwdVec.X;//  # determinant
	auto radians = FMath::Atan2(det, dot); // # atan2(y, x) or atan2(sin, cos)
	auto degrees = FMath::RadiansToDegrees(radians);

	return FMath::Abs(degrees);
}
