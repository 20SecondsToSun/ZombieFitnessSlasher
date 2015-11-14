// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "enemy/EnemyCharacter.h"
#include "EnemyAnimInstance.generated.h"


UCLASS()
class NETWORKRUNGAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		bool IsAppereanceComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		bool IsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		float Speed;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		int32 AttackingPoseNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		int32 DeathPoseNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		int32 DEATH_POSE_COUNT = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = AnimationState)
		int32 ATTACK_POSE_COUNT = 2;


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = AnimationState)
		virtual void SetAttacking(bool value);

	bool SetAttacking_Validate(bool value) { return true; };
	void SetAttacking_Implementation(bool value);


	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation, Category = AnimationState)
		virtual void CheckIsAlive();

	bool CheckIsAlive_Validate() { return true; };
	void CheckIsAlive_Implementation();


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = AnimationState)
		virtual void CheckAttackingPose();

	bool CheckAttackingPose_Validate() { return true; };
	void CheckAttackingPose_Implementation();


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = AnimationState)
		virtual void SetSpeed();

	bool SetSpeed_Validate() { return true; };
	void SetSpeed_Implementation();


	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

private:
	void GenereateDeathAnimationNum();
	void GenereateAttackAnimationNum();

	AEnemyCharacter* EnemyCharacter;
	
};
