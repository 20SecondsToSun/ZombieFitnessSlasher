// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "FitCharacter.h"
#include "Engine.h"
#include "Player/Weapon/Guns/BigBangGun.h"
#include "Player/Weapon/Projectiles/BigBangGunProjectile.h"
#include "Player/Weapon/Muzzles/BigBangGunExplotion.h"
#include "Player/PickupStuff/PickupStuffBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/FitPlayerState.h"
#include "Player/FitPlayerController.h"
#include "World/FitGameState.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/EnemyActor.h"
#include "Components/AimComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tools/Math/Math.h"
#include "Player/weapon/Ammo/Ammo.h"

// Sets default values
AFitCharacter::AFitCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	RoadSplineComponent = CreateDefaultSubobject<URoadSplineComponent>(TEXT("RoadSplineComponent"));
	
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetIsReplicated(true);
	WeaponComponent->SetNetAddressable();
	
	PickupStuffComponent = CreateDefaultSubobject<UPickupStuffComponent>(TEXT("PickupStuffComponent"));
	PickupStuffComponent->SetIsReplicated(true);
	PickupStuffComponent->SetNetAddressable();


	static ConstructorHelpers::FClassFinder<AActor> MainWeaponBlueprint(TEXT("/Game/game/Weapon/Blueprints/BigBangGun_BP"));
	if (MainWeaponBlueprint.Succeeded())
	{
		MainWeapon = MainWeaponBlueprint.Class;		
	}
	
	//
	
	SetActorEnableCollision(true);
	GetCapsuleComponent()->bGenerateOverlapEvents = true;		

	MAX_PLAYER_VELOCITY = 1.0f;
	GYPER_PLAYER_VELOCITY = 1.0f;
	MAX_DISTANCE_FOR_SHOOT = 15.0f;
	MAX_PENALTY_SECONDS = 7.0f;	
	
}

void AFitCharacter::ResetParams_Implementation()
{
	WeaponComponent->SetMainWeaponPatrons(10);
	RoadSplineComponent->ResetProgress();
}

void AFitCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

void AFitCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		WeaponComponent->SetMainWeapon(MainWeapon, GetMesh());
		WeaponComponent->SetMainWeaponPatrons(10);
		SetMaxVelocity(MAX_PLAYER_VELOCITY);
		SetCurrentVelocity(0.5f);
		//AttachWeapon();

		GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFitCharacter::OnCharacterHitHandler);
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFitCharacter::OnCharacterBeginOverlapHandler);
	}
}

void AFitCharacter::OnCharacterHitHandler(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

void AFitCharacter::OnCharacterBeginOverlapHandler(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PickupStuffComponent->Pickup(OtherActor);
}

void AFitCharacter::SetTrackID_Implementation(int32 value)
{
	RoadSplineComponent->SetActiveTrackID(value);
}

void AFitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (RoadSplineComponent)
	{
		auto controller = Cast<AFitPlayerController>(GetController());
		if (!controller)
			return;

		if (controller->FollowSpline)
		{
			RoadSplineComponent->MoveCharacterlongSplineTrack(this);
			SetDistanceOnSpline(RoadSplineComponent->GetPlayerSplineDistancePoint());
		}

		//if (!HasAuthority())
			CheckAimInFieldOfView();

		if (CameraIsFollowing)
		{
			if (controller->IsCameraFollow)
			{
				FRotator cameraRotation;
				cameraRotation.Yaw   = GetActorRotation().Yaw;
				cameraRotation.Roll  = GetActorRotation().Roll;
				cameraRotation.Pitch = -5;
				//FollowCamera->SetWorldRotation(cameraRotation);
				CameraBoom->SetWorldRotation(GetActorRotation());
				//CameraBoom->SocketOffset = FVector(0, 0, 100);
			}		
		}		
	}				
}

void AFitCharacter::SetDistanceOnSpline_Implementation(float value)
{
	DistanceOnTrack = value;
}

//============================================================================
//* 
//*					 ATTACKING
//* 
//=============================================================================

void AFitCharacter::AddFrag_Implementation()
{
	Frags++;
}

void AFitCharacter::CheckAimInFieldOfView_Implementation()
{
	SetEnemyAsAim();	
}

void AFitCharacter::SetEnemyAsAim()
{	
	float MinDist = 5000;	
	TArray<AActor*> enemies;	
	AActor* candidate = nullptr;
	UAimComponent* AimCandidate = nullptr;

	auto World = GetWorld();

	if (World)
	{		
		TArray<TEnumAsByte<enum EObjectTypeQuery>> DesiredObjects;
		DesiredObjects.Add(EObjectTypeQuery(ECC_Pawn));
		DesiredObjects.Add(EObjectTypeQuery(ECC_WorldStatic));

		TArray<AActor*> IgnoredActors;
		TArray<FHitResult> HitResults;
		
		UKismetSystemLibrary::SphereTraceMultiForObjects(World, 
			GetActorLocation() + GetActorForwardVector() * 2500,
			GetActorLocation() + GetActorForwardVector() * 4000,
			2000,
			DesiredObjects,
			false, 
			IgnoredActors, 
			EDrawDebugTrace::None,
			HitResults,
			true);		

		for (size_t i = 0; i < HitResults.Num(); i++)
		{
			auto HitActor = HitResults[i].GetActor();
			TArray<UAimComponent*>  HighlightComponents;
			HitActor->GetComponents(HighlightComponents);

			for (UAimComponent* Aim : HighlightComponents)
			{
				if (Aim && Aim->CanBeAimed())
				{				
					auto Dist = FVector::Dist(GetActorLocation(), HitActor->GetActorLocation());

					if (MinDist > Dist && Math::CalculateAngleBetweenActorsDeg(this, HitActor) < 90)
					{
						MinDist = Dist;
						candidate = HitActor;
						AimCandidate = Aim;
					}
				}
			}
		}		
	}

	FEnemyAimStruct AimStruct;

	if (candidate)
	{	
		AimCandidate->SetDistance_Implementation(MinDist);
		AimStruct.DistanceTo = AimCandidate->DistanceTo;		
	}

	AimStruct.AnemyAim = AimCandidate;
	SetAimInFeldOfView_Implementation(AimStruct);
}

void AFitCharacter::SetAimInFeldOfView_Implementation(FEnemyAimStruct EnemyAim)
{
	AFitPlayerState* FitPlayerState = Cast<AFitPlayerState>(PlayerState);

	if (FitPlayerState)
	{
		FitPlayerState->SetHighLightsOFF();
		FitPlayerState->SetAim(EnemyAim);
		FitPlayerState->SetHighLightsON();			
	}		
}

bool AFitCharacter::CheckHasAnEnemyAim()
{
	AFitPlayerState* FitPlayerState = Cast<AFitPlayerState>(PlayerState);
	return FitPlayerState && FitPlayerState->GetAnemyAim();
}

void AFitCharacter::Fire_Implementation()
{
	if (CheckHasAnEnemyAim())
	{
		if (WeaponComponent->CanFire() && !HasUserInputDelay)
		{
			WeaponComponent->Fire();
			IsFire = true;

			SetUserInputDelay(true);

			GetWorldTimerManager().ClearTimer(ShootTimer);
			FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitCharacter::ShootTimerComplete);
			GetWorldTimerManager().SetTimer(ShootTimer, del, 5.0f, true);
		}	
	}
}

void AFitCharacter::ShootTimerComplete()
{
	GetWorldTimerManager().ClearTimer(ShootTimer);
	FireStop();
}

void AFitCharacter::FireStop_Implementation()
{
	IsFire = false;
}

//void AFitCharacter::AddPatrons_Implementation(int32 count, int32 type)
//{
//	Patrons[type] += count;
//}
//
//void AFitCharacter::RemovePatrons_Implementation(int32 count, int32 type)
//{
//	if (Patrons[type] > 0)
//		Patrons[type] -= count;
//}

//============================================================================
//* 
//*					INTERFACE BINDING
//* 
//============================================================================
 
void AFitCharacter::UpdateStateForInterface_Implementation(EGraphicInterfaceEnum state)
{
	GetWorldTimerManager().ClearTimer(GrInterfaceTimer);

	if (state == EGraphicInterfaceEnum::VE_FREEZE || state == EGraphicInterfaceEnum::VE_GUN)
	{
		if (GrInterfaceEnum == EGraphicInterfaceEnum::VE_DAMAGE || GrInterfaceEnum == EGraphicInterfaceEnum::VE_RUN_RUN)
			return;
		else
		{
			FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitCharacter::GrInterfaceTimerHandler);
			GetWorldTimerManager().SetTimer(GrInterfaceTimer, del, 1.0, true);
		}
	}
	GrInterfaceEnum = state;
}

void AFitCharacter::GrInterfaceTimerHandler()
{
	UpdateStateForInterface(EGraphicInterfaceEnum::VE_NONE);
}

void AFitCharacter::SetScreenLocation_Implementation(const FVector2D& value)
{
	ScreenLocation = value;
}

//============================================================================
//* 
//*					INPUT AND MOVING
//* 
//============================================================================

void AFitCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
		
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AFitCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFitCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AFitCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AFitCharacter::LookUpAtRate);
	
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFitCharacter::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &AFitCharacter::FireStop);

	//InputComponent->BindAction("Throw", IE_Pressed, this, &AFitCharacter::Throw);
	//InputComponent->BindAction("Throw", IE_Released, this, &AFitCharacter::ThrowStop);

	InputComponent->BindAction("OnLeft", IE_Released, this, &AFitCharacter::OnLeft);
	InputComponent->BindAction("OnRight", IE_Released, this, &AFitCharacter::OnRight);

	InputComponent->BindAction("Velocity1", IE_Pressed, this, &AFitCharacter::setMaxVelocityT<1>);
	InputComponent->BindAction("Velocity2", IE_Pressed, this, &AFitCharacter::setMaxVelocityT<2>);
	InputComponent->BindAction("Velocity3", IE_Pressed, this, &AFitCharacter::setMaxVelocityT<3>);
	InputComponent->BindAction("Velocity4", IE_Pressed, this, &AFitCharacter::setMaxVelocityT<4>);
	InputComponent->BindAction("Velocity5", IE_Pressed, this, &AFitCharacter::setMaxVelocityT<5>);
	InputComponent->BindAction("Velocity6", IE_Pressed, this, &AFitCharacter::setMaxVelocityT<6>);
}

template<int velocity>
void AFitCharacter::setMaxVelocityT()
{
	if (Controller != NULL)
	{
		switch (velocity)
		{
			case 1:	SetMaxVelocity(0.1f);	break;
			case 2:	SetMaxVelocity(0.2f);	break;
			case 3:	SetMaxVelocity(0.3f);	break;
			case 4:	SetMaxVelocity(0.4f);	break;
			case 5:	SetMaxVelocity(0.5f);	break;
			case 6:	SetMaxVelocity(1.0f);	break;
		}
	}
}

void AFitCharacter::SetMaxVelocity_Implementation(float Value)
{
	MaxVelocity = Value;
	SetCurrentVelocity(Value);
}

void AFitCharacter::SetCurrentVelocity_Implementation(float Value)
{
	CurrentVelocity = Value;
}

void AFitCharacter::OnLeft()
{
	if (/*Spline &&*/ !HasUserInputDelay)
	{
		RoadSplineComponent->TurnToLeft();
		SetUserInputDelay(true);
	}
}

void AFitCharacter::OnRight()
{
	if (/*Spline && */!HasUserInputDelay)
	{
		RoadSplineComponent->TurnToRight();
		SetUserInputDelay(true);
	}
}

void AFitCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFitCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFitCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//CurrentVelocity = Value;
		AddCustomMovementInput(Direction);
	}		
}

void AFitCharacter::AddCustomMovementInput(const FVector& Direction)
{
	auto value = FMath::Abs(CurrentVelocity);
	AddMovementInput(Direction, value);
}

void AFitCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFitCharacter::SetUserInputDelay_Implementation(bool value)
{
	HasUserInputDelay = value;

	if (HasUserInputDelay)
	{
		FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AFitCharacter::InputDelayHandler);
		GetWorldTimerManager().SetTimer(InputDelayTimer, del, 0.5f, true);
	}	
}

void AFitCharacter::InputDelayHandler()
{
	GetWorldTimerManager().ClearTimer(InputDelayTimer);
	SetUserInputDelay(false);
}

//============================================================================
//* 
//*					ARDUINO		 
//* 
//=============================================================================


void AFitCharacter::SetArduinoResistance(int32 Resistancevalue, float Time)
{
	auto controller = Cast<AFitPlayerController>(GetController());

	if (controller)
	{
		controller->SetArduinoResistance(Resistancevalue, Time);
	}
}

//============================================================================
//* 
//*					GETTERS
//* 
//=============================================================================

float AFitCharacter::GetCurrentVelocity() const
{
	return CurrentVelocity;
}

bool AFitCharacter::IsWin() const
{
	return RoadSplineComponent->IsNextLoop();
}

float AFitCharacter::GetRemainMeters()
{
	return RoadSplineComponent->GetRemainMeters();
}

//============================================================================
//* 
//*					REPLICATION
//* 
//=============================================================================

void AFitCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFitCharacter, IsFire);
	DOREPLIFETIME(AFitCharacter, MaxVelocity);
	DOREPLIFETIME(AFitCharacter, CurrentVelocity);
	//DOREPLIFETIME(AFitCharacter, Patrons);
	DOREPLIFETIME(AFitCharacter, Health);
	DOREPLIFETIME(AFitCharacter, AimInFieldOfView);
	DOREPLIFETIME(AFitCharacter, ScreenLocation);
	DOREPLIFETIME(AFitCharacter, DistanceOnTrack);
	DOREPLIFETIME(AFitCharacter, HasUserInputDelay);
	DOREPLIFETIME(AFitCharacter, Frags);
}
