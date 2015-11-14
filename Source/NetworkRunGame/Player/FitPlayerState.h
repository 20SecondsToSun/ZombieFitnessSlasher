// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Components/AimComponent.h"
#include "FitPlayerState.generated.h"

USTRUCT(BlueprintType)
struct NETWORKRUNGAME_API FEnemyAimStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DistanceTo;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SeeAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAimComponent* AnemyAim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsSet;

	FEnemyAimStruct()
	{
		AnemyAim = nullptr;  //You must init this to avoid crashes	
		IsSet = false;
	}
};

UCLASS()
class NETWORKRUNGAME_API AFitPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Withvalidation, Reliable)
		void SetAim(FEnemyAimStruct EnemyAim);
	void SetAim_Implementation(FEnemyAimStruct EnemyAim);
	bool SetAim_Validate(FEnemyAimStruct EnemyAim){ return true; };

	UFUNCTION(Client, Reliable)
		void SetHighLightsON();
	void SetHighLightsON_Implementation();

	UFUNCTION(Client, Reliable)
		void SetHighLightsOFF();
	void SetHighLightsOFF_Implementation();

	bool IsAimClose();
	bool IfAimEquals(FEnemyAimStruct EnemyAim);
	void SetEnemyAimOnFire();

	UAimComponent* GetAnemyAim();

private:
	UPROPERTY(Replicated)
		FEnemyAimStruct EnemyAimStruct;

	UPROPERTY(Replicated)
		FEnemyAimStruct PrevEnemyAimStruct;

	UPROPERTY(Replicated)
		int32 Player_ID;

	UPROPERTY(Replicated)
		int32 TrackID = 3;

	UPROPERTY(Replicated)
		FColor PlayerColor;
};

