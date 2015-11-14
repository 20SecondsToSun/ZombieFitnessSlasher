// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/AimComponent.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class NETWORKRUNGAME_API AEnemyCharacter : public ACharacter 
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "EnemyBehaviour")
	virtual void StartHeroAttack();

	bool StartHeroAttack_Validate( ) { return true; };
	void StartHeroAttack_Implementation()
	{
		IsAttacking = true;
	};

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "EnemyBehaviour")
	virtual void StopHeroAttack();

	bool StopHeroAttack_Validate() { return true; };
	void StopHeroAttack_Implementation() 
	{
		IsAttacking = false;
	};

	UFUNCTION(BlueprintCallable, Category = "EnemyBehaviour")
		virtual void TryToDamageHero(AActor* DamageActor);

	UFUNCTION(Server, Reliable, WithValidation, Category = "EnemyBehaviour")
		virtual void DamageApply(const FVector& Location);

	bool DamageApply_Validate(const FVector& Location) { return true; };
	void DamageApply_Implementation(const FVector& Location);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "EnemyInfo")
		bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "EnemyInfo")
		bool _IsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "EnemyInfo")
		int32 Health = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "EnemyInfo")
		UBoxComponent* Basa;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "EnemyInfo")
		ACharacter* ChasingCharacter;		

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Aim")
		UAimComponent* AimComponent;

	bool IsAlive();

private:
	UClass* FXImpactClass;
};
