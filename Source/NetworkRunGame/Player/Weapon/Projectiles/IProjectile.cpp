// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "IProjectile.h"
#include "Engine.h"


// Sets default values
AIProjectile::AIProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereColllisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColllisionComponent"));
	SphereColllisionComponent->InitSphereRadius(35.0f);

	MissleSkeletalMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissleSkeletalMeshComp"));	

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 2000.0f;
	ProjectileMovementComp->MaxSpeed = 3000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComp->bIsHomingProjectile = true;
	ProjectileMovementComp->HomingAccelerationMagnitude = 40000.0f;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));

	RootComponent = SphereColllisionComponent;
	MissleSkeletalMeshComp->AttachTo(SphereColllisionComponent);
}

// Called when the game starts or when spawned
void AIProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AIProjectile::MulticastRocketLaunch_Implementation(UAimComponent* AimComponent)
{
	if (ProjectileMovementComp && AimComponent)
	{
		ProjectileMovementComp->HomingTargetComponent = AimComponent->GetSceneLinkedComponent();
		Aim = AimComponent;
	}
}

void AIProjectile::AddOnComponentHitHandlerServer()
{
	if (SphereColllisionComponent)
	{
		SphereColllisionComponent->OnComponentHit.AddDynamic(this, &ABigBangGunProjectile::OnComponentHitHandlerServer);
	}
}

void AIProjectile::AddOnComponentHitHandlerClient()
{
	if (SphereColllisionComponent)
	{
		SphereColllisionComponent->OnComponentHit.AddDynamic(this, &ABigBangGunProjectile::OnComponentHitHandlerClient);
	}
}

void AIProjectile::OnComponentHitHandlerServer(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp && HasAuthority())
	{
		if (OtherActor->IsA(AIProjectile::StaticClass()))
		{
			Destroy();
			return;
		}
		else if (OtherActor->IsA(AFitCharacter::StaticClass()))
		{
			return;
		}
		else if (OtherActor->IsA(AEnemyCharacter::StaticClass()))
		{
			auto Enemy = Cast<AEnemyCharacter>(OtherActor);

			if (Enemy && Enemy->IsAlive())
			{
				Enemy->DamageApply(SphereColllisionComponent->GetComponentLocation());
				if (Owner)
				{
					Owner->AddFrag();
				}
			}
		}
		else if (OtherActor->IsA(AEnemyActor::StaticClass()))
		{
			auto Enemy = Cast<AEnemyActor>(OtherActor);
			if (Enemy && Enemy->IsAlive())
			{
				Enemy->DamageApply(SphereColllisionComponent->GetComponentLocation());
			}
		}

		if (Aim)
			Aim->OnFire = false;

		DefaultExplode();
		Destroy();
	}
}

void AIProjectile::OnComponentHitHandlerClient(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DefaultExplode();
	Destroy();
}

void AIProjectile::DefaultExplode()
{

}

void AIProjectile::SetPlayerOwnerID(int32 id)
{
	PlayerOwnerID = id;
}

int32 AIProjectile::GetPlayerOwnerID() const
{
	return PlayerOwnerID;
}

void AIProjectile::SetOwner(AFitCharacter* SelfPlayer)
{
	Owner = SelfPlayer;
}

void AIProjectile::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABigBangGunProjectile, Id);
}
