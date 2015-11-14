// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "EnemyAnimInstance.h"
#include "Net/UnrealNetwork.h"

#include "Engine.h"

void UEnemyAnimInstance::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEnemyAnimInstance, IsAttacking);
	DOREPLIFETIME(UEnemyAnimInstance, IsAppereanceComplete);
	DOREPLIFETIME(UEnemyAnimInstance, IsAlive);
	
	DOREPLIFETIME(UEnemyAnimInstance, AttackingPoseNum);
	DOREPLIFETIME(UEnemyAnimInstance, DeathPoseNum);

	DOREPLIFETIME(UEnemyAnimInstance, Direction);
	DOREPLIFETIME(UEnemyAnimInstance, Speed);
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();
	EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	SetSpeed();
	CheckIsAlive();
	CheckAttackingPose();
}

void UEnemyAnimInstance::SetSpeed_Implementation()
{
	if (EnemyCharacter && IsAlive)
	{
		Speed = EnemyCharacter->GetVelocity().Size();
	}
}

void UEnemyAnimInstance::SetAttacking_Implementation(bool value)
{
	IsAttacking = value;

	if (!value)
	{
		if (EnemyCharacter)
		{
			EnemyCharacter->StopHeroAttack();
		}
	}	
}

void UEnemyAnimInstance::CheckIsAlive_Implementation()
{
	if (EnemyCharacter && IsAlive)
	{
		if (!EnemyCharacter->IsAlive())
		{
			Speed = 0.0f;
			GenereateDeathAnimationNum();
			IsAlive = false;	
		}
	}
}

void UEnemyAnimInstance::GenereateDeathAnimationNum()
{
	DeathPoseNum = FMath::FRandRange(0, DEATH_POSE_COUNT);
}

void UEnemyAnimInstance::CheckAttackingPose_Implementation()
{
	if (EnemyCharacter && IsAlive && !IsAttacking)
	{
		if (EnemyCharacter->IsAttacking)
		{
			GenereateAttackAnimationNum();
			IsAttacking = true;
		}
	}
}

void UEnemyAnimInstance::GenereateAttackAnimationNum()
{
	AttackingPoseNum = FMath::FRandRange(0, ATTACK_POSE_COUNT);
}
