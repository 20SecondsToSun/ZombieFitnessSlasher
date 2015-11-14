// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Player/Weapon/Guns/IWeapon.h"
#include "Tools/FitTypes.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKRUNGAME_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void SetMainWeapon(UClass* Weapon, USkeletalMeshComponent* CharacterMesh);

	bool CanFire();
	void Fire();

	void AddMainWeaponPatrons();

	
	UFUNCTION(Server, WithValidation, Reliable)
	void AddMainWeaponPatrons(int32 PatronNums);
	void AddMainWeaponPatrons_Implementation(int32 value);
	bool AddMainWeaponPatrons_Validate(int32 value){ return true; };



	void SetAdditionalWeapon(AIWeapon* AdditionalWeapon);



	void RemoveCurrentWeaponPatrons();
	void SetMainWeaponPatrons(int32 PatronsNum);



	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetCurrentPatronsNum();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere)
	AIWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere)
	AIWeapon* MainWeapon;

	UPROPERTY(EditAnywhere)
	AIWeapon* AdditionWeapon;

	UPROPERTY(Replicated)
	int32 MainWeaponPatrons = 0;

	UPROPERTY(Replicated)
	int32 AdditionalWeaponPatrons = 0;

	UPROPERTY(Replicated)
	EWeaponCharacterEnum WeaponType;

	FName AttachSocketName = FName("GunSocket");


	void ResetMainWeaponHandler();
	FTimerHandle ResetMainWeaponTimer;


	
};
