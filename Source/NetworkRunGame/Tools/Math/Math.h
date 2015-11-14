// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class NETWORKRUNGAME_API Math
{
public:
	static float angleBetweenVectors(const FVector& v1, const FVector& v2);
	static FRotator Math::RotBetweenVect(const FVector& A, const FVector& B);
	static float CalculateAngleBetweenActorsDeg(AActor* CHaracterActor, AActor* AimActor);
	
	template <typename T>
	static FORCEINLINE TArray<T> SuffleArray(TArray<T> array)
	{
		array.Sort([](const T Item1, const T Item2) {return FMath::FRand() < 0.5f; });
		return  array;
	}
};
