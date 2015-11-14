// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Player/Weapon/Guns/IWeapon.h"
#include "Components/RoadSplineComponent.h"
#include "Tools/FitTypes.h"
#include "Player/Weapon/WeaponEnum.h"
#include "Player/FitPlayerState.h"
#include "Components/AimComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/PickupStuffComponent.h"
#include "FitCharacter.generated.h"

UCLASS()
class NETWORKRUNGAME_API AFitCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:	
	AFitCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;	

	UFUNCTION(BlueprintCallable, Category = "Attacking")
		bool CheckHasAnEnemyAim();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const override;	

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const  { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		EGraphicInterfaceEnum GrInterfaceEnum;

	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable, Category = "Player Controll")
		void Fire();

	UFUNCTION(Server, WithValidation, Reliable)
		void FireStop();

	UFUNCTION(Server, WithValidation, Reliable)
		void SetUserInputDelay(bool value);	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Fire")
		bool IsFire = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		float MaxVelocity = 0.7f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		float CurrentVelocity = 0.0f;	

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	//	TArray<int32> Patrons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		int32 Health = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		int32 Frags = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		int32 DistanceOnTrack = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		bool AimInFieldOfView = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		FVector2D ScreenLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
		bool HasUserInputDelay = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MAX_PLAYER_VELOCITY = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float GYPER_PLAYER_VELOCITY = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MAX_DISTANCE_FOR_SHOOT = 15.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MAX_PENALTY_SECONDS = 7.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		bool CameraIsFollowing = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
		URoadSplineComponent* RoadSplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PickUp")
		UPickupStuffComponent* PickupStuffComponent;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		UClass* MainWeapon;

	UClass* PickupWeapon;


	//UFUNCTION(Server, WithValidation, Reliable)
		//void AddPatrons(int32 count, int32 type);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetDistanceOnSpline(float value);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetAimInFeldOfView(FEnemyAimStruct EnemyAim);
	void SetAimInFeldOfView_Implementation(FEnemyAimStruct _EnemyAim);
	bool SetAimInFeldOfView_Validate(FEnemyAimStruct _EnemyAim){ return true; };


	UFUNCTION(Server, WithValidation, Reliable)
		void SetScreenLocation(const FVector2D& value);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetMaxVelocity(float velocity);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetCurrentVelocity(float velocity);

	UFUNCTION(Server, WithValidation, Reliable)
		void AddFrag();	

	//void AttachWeapon();
	float GetCurrentVelocity() const;		
	
	UFUNCTION(Server, Withvalidation, Reliable)
		void CheckAimInFieldOfView();

	UFUNCTION(Client, Reliable)
		void SetTrackID(int32 value);		

	void AddCustomMovementInput(const FVector& Direction);

	UFUNCTION(BlueprintCallable, Category = "Track Info")
		float GetRemainMeters();

	UFUNCTION(Client, Reliable)
		void UpdateStateForInterface(EGraphicInterfaceEnum state);	

	UFUNCTION(BlueprintCallable, Category = "Player Controll")
		void OnLeft();

	UFUNCTION(BlueprintCallable, Category = "Player Controll")
		void OnRight();		

	UFUNCTION(BlueprintCallable, Category = "GamePlay")
		bool IsWin() const;

	UFUNCTION(Client, Reliable)
		void ResetParams();

	UFUNCTION()
	void OnCharacterHitHandler(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnCharacterBeginOverlapHandler(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

private:
	//AIWeapon* FireWeapon;
	FTimerHandle GrInterfaceTimer, InputDelayTimer, ShootTimer;

	int32 DistanceBetween;

	void InputDelayHandler();	
	void GrInterfaceTimerHandler();
	void ShootTimerComplete();
	
	template<int Index>
	void setMaxVelocityT();

	//============================================================================
	//* 
	//* 
	//=============================================================================

	void Fire_Implementation();
	bool Fire_Validate() { return true; };

	//============================================================================
	//* 
	//* 
	//=============================================================================

	void FireStop_Implementation();
	bool FireStop_Validate() { return true; };

	//============================================================================
	//* 
	//* 
	//=============================================================================	

	void SetMaxVelocity_Implementation(float velocity);
	bool SetMaxVelocity_Validate(float velocity) { return true; };

	//============================================================================
	//* 
	//* 
	//=============================================================================
	
	void SetCurrentVelocity_Implementation(float velocity);
	bool SetCurrentVelocity_Validate(float velocity) { return true; };	

	//============================================================================
	//* 
	//* 
	//=============================================================================
	//void AddPatrons_Implementation(int32 count, int32 type);
	//bool AddPatrons_Validate(int32 count, int32 type) { return true; };

	//============================================================================
	//* 
	//* 
	//=============================================================================
	//UFUNCTION(Server, WithValidation, Reliable)
	
	

	//void RemovePatrons_Implementation(int32 count, int32 type);
	//bool RemovePatrons_Validate(int32 count, int32 type) { return true; };

	//============================================================================
	//* 
	//* 
	//=============================================================================

	void SetTrackID_Implementation(int32 value);
	bool SetTrackID_Validate(int32 value){ return true; };

	//============================================================================
	//* 
	//* 
	//=============================================================================

	void SetPlayerColor_Implementation(const FColor& PlayerColor);
	bool SetPlayerColor_Validate(const FColor& PlayerColor){ return true; };


	//============================================================================
	//* 
	//* 
	//=============================================================================

	void CheckAimInFieldOfView_Implementation();
	bool CheckAimInFieldOfView_Validate(){ return true; };

	//============================================================================
	//* 
	//* 
	//=============================================================================

	void SetDistanceOnSpline_Implementation(float value);
	bool SetDistanceOnSpline_Validate(float value){ return true; };	

	//============================================================================
	//* 
	//* 
	//=============================================================================

	void SetScreenLocation_Implementation(const FVector2D& value);
	bool SetScreenLocation_Validate(const FVector2D& value){ return true; };

	//============================================================================
	//* 
	//* 
	//=============================================================================

	void SetUserInputDelay_Implementation(bool value);
	bool SetUserInputDelay_Validate(bool value){ return true; };

	void AddFrag_Implementation();
	bool AddFrag_Validate(){ return true; };

	void SetArduinoResistance(int32 value, float Time);
	void SetEnemyAsAim();
};
