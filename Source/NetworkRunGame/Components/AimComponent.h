// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "AimComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKRUNGAME_API UAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAimComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyVisual")
		TArray<UMaterialInstanceDynamic*> MaterialDynamicInstances;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "EnemyInfo")
		bool OnFire = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "EnemyInfo")
		float DistanceTo = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "EnemyInfo")
		bool IsAlive = true;

	UFUNCTION(Client, Reliable)
		void SetColorHighlight();
	void SetColorHighlight_Implementation();

	UFUNCTION(Client, Reliable)
		void SetDefaultHighlight();
	void SetDefaultHighlight_Implementation();

	UFUNCTION(Server, Reliable, WithValidation, Category = "EnemyBehaviour")
		void SetEnemyAimOnFire();
	bool SetEnemyAimOnFire_Validate() { return true; };
	void SetEnemyAimOnFire_Implementation();

	UFUNCTION(Server, Reliable, WithValidation, Category = "EnemyBehaviour")
		void SetDistance(float Distance);
	bool SetDistance_Validate(float Distance) { return true; };
	void SetDistance_Implementation(float Distance);

	UFUNCTION(Server, Reliable, WithValidation, Category = "EnemyBehaviour")
		void SetSceneLinkedComponent(USceneComponent* value);	
	bool  SetSceneLinkedComponent_Validate(USceneComponent* value) { return true; };
	void  SetSceneLinkedComponent_Implementation(USceneComponent* value);

	bool CanBeAimed();
	bool IsClose();
	void ResetOnFire();
	USceneComponent* GetSceneLinkedComponent();

private:
	UPROPERTY(Replicated)
		USceneComponent* SceneLinkedComponent;
};
