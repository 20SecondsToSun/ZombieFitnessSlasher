// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "IWeapon.generated.h"

UCLASS()
class NETWORKRUNGAME_API AIWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AIWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* WeaponSkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UClass* ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* FlashParticleSystemRaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* ParticleSystemComponent;

//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	class USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ID = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PatronsNum = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttached = false;


	UFUNCTION(NetMulticast, WithValidation, Reliable)
	virtual void Fire(AFitCharacter* selfPlayer);
	void Fire_Implementation(AFitCharacter* selfPlayer);
	bool Fire_Validate(AFitCharacter* selfPlayer) { return true; };

	void SetAssets(const FString& SkeletMeshPath, const FString& ProjectileBPPath, const FString& ParticleSystemPath);


private:
	FTimerHandle FlashTimer;
	void FlashEmittHandler();	
	void FireOnDelay(AFitCharacter* SelfPlayer);
};
