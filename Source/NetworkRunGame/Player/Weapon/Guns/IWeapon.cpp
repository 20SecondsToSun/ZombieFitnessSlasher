// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "IWeapon.h"
#include "Engine.h"

AIWeapon::AIWeapon()
{
 	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	WeaponSkeletalMeshComp	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponSkeletalMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponSkeletalMeshComp->bGenerateOverlapEvents = true;
	WeaponSkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	ArrowComponent			= CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));	

	RootComponent = WeaponSkeletalMeshComp;
}

void AIWeapon::SetAssets(const FString& SkeletMeshPath, const FString& ProjectileBPPath, const FString& ParticleSystemPath)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalAsset(*SkeletMeshPath);
	if (SkeletalAsset.Succeeded())
	{
		WeaponSkeletalMeshComp->SetSkeletalMesh(SkeletalAsset.Object);
	}

	static ConstructorHelpers::FClassFinder<AActor> ProjectileBlueprint(*ProjectileBPPath);
	if (ProjectileBlueprint.Succeeded())
	{
		ProjectileClass = ProjectileBlueprint.Class;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlashAsset(*ParticleSystemPath);
	if (FlashAsset.Succeeded())
	{
		FlashParticleSystemRaw = FlashAsset.Object;
	}
}

void AIWeapon::Fire_Implementation(AFitCharacter* SelfPlayer)
{
	FTimerDelegate del = FTimerDelegate::CreateUObject(this, &AIWeapon::FireOnDelay, SelfPlayer);
	GetWorldTimerManager().SetTimer(FlashTimer, del, 0.2f, false);	
}

void AIWeapon::FireOnDelay(AFitCharacter* SelfPlayer)
{
	if (SelfPlayer)
	{
		auto FitPlayerState = Cast<AFitPlayerState>(SelfPlayer->PlayerState);

		if (FitPlayerState)
		{
			auto Aim = FitPlayerState->GetAnemyAim();
			auto World = GetWorld();

			if (World && ProjectileClass && Aim)
			{
				FRotator SpawnDirection = SelfPlayer->GetControlRotation();

				auto FwdVector = SelfPlayer->GetActorForwardVector();
				//	FwdVector.Normalize();
				FwdVector *= 400;

				auto SpawnPosition = SelfPlayer->GetActorLocation() + FwdVector;
				auto Missle = World->SpawnActor<AIProjectile>(ProjectileClass, SpawnPosition, SpawnDirection);


				FlashEmittHandler();

				if (HasAuthority())
				{
					Missle->AddOnComponentHitHandlerServer();
				}
				else
				{
					Missle->AddOnComponentHitHandlerClient();
				}

				auto Controller = SelfPlayer->GetController();
				if (Controller)
				{
					AFitPlayerState* State = Cast<AFitPlayerState>(Controller->PlayerState);
					if (State)
					{
						Missle->Id = State->PlayerId;
					}
				}

				if (Missle)
				{
					FitPlayerState->SetEnemyAimOnFire();
					Missle->SetOwner(SelfPlayer);
					Missle->MulticastRocketLaunch(Aim);
				}
			}
		}
	}
}

void AIWeapon::FlashEmittHandler()
{
	if (FlashParticleSystemRaw)
	{		
		auto Scale = ParticleSystemComponent->RelativeScale3D;
		auto Rotor = ParticleSystemComponent->RelativeRotation;					
		
		auto _ParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(FlashParticleSystemRaw,
			RootComponent, NAME_None, ArrowComponent->RelativeLocation, Rotor,
			EAttachLocation::KeepRelativeOffset, true);	

		if (_ParticleSystemComponent)
		{
			_ParticleSystemComponent->SetWorldScale3D(Scale);
			_ParticleSystemComponent->SetWorldRotation(Rotor);
		}			
	}	
}
