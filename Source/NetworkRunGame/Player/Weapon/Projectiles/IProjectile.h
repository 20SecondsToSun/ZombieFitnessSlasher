// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Player/FitCharacter.h"
#include "Components/AimComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "IProjectile.generated.h"

UCLASS()
class NETWORKRUNGAME_API AIProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MissleSkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class URadialForceComponent* RadialForceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* SphereColllisionComponent;


	UFUNCTION(Server, WithValidation, Reliable)
		void MulticastRocketLaunch(UAimComponent* Actor);
	bool MulticastRocketLaunch_Validate(UAimComponent* Actor) { return true; };
	void MulticastRocketLaunch_Implementation(UAimComponent* Actor);

	//UFUNCTION(BlueprintImplementableEvent, Category = "HomingMissleEvent")
	//	void HomingMissleEvent(USceneComponent* actor);

	UFUNCTION()
		void OnComponentHitHandlerServer(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnComponentHitHandlerClient(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void AddOnComponentHitHandlerServer();
	void AddOnComponentHitHandlerClient();

	int32 GetPlayerOwnerID() const;
	void SetPlayerOwnerID(int32 id);
	void SetOwner(AFitCharacter* SelfPlayer);

protected:
	int32 PlayerOwnerID;
	AFitCharacter* Owner;
	UAimComponent* Aim;

	void DefaultExplode();	
};
