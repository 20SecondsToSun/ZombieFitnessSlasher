// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "PickupStuffComponent.h"
#include "Player/FitCharacter.h"
#include "Player/FitPlayerController.h"
#include "Player/PickupStuff/RunArrowStuff.h"


// Sets default values for this component's properties
UPickupStuffComponent::UPickupStuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	bReplicates = true;
	// ...
}


// Called when the game starts
void UPickupStuffComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...	
}


// Called every frame
void UPickupStuffComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UPickupStuffComponent::Pickup(AActor* Stuff)
{	
	if (Stuff->IsA(AAmmo::StaticClass()))
	{
		Stuff->Destroy();
		auto CharacterOwner = Cast<AFitCharacter>(GetOwner());
		
		if (CharacterOwner)
		{
			auto Ammo = Cast<AAmmo>(Stuff);		
			CharacterOwner->WeaponComponent->AddMainWeaponPatrons(Ammo->PatronsNum);			
		}
	}
	else if (Stuff->IsA(AIWeapon::StaticClass()))
	{
		//Stuff->Destroy();

		auto CharacterOwner = Cast<AFitCharacter>(GetOwner());

		if (CharacterOwner)
		{
			auto AdditionalWeapon = Cast<AIWeapon>(Stuff);
			if (!AdditionalWeapon->IsAttached)
			{			

				AdditionalWeapon->IsAttached = true;
				CharacterOwner->WeaponComponent->SetAdditionalWeapon(AdditionalWeapon);
			}			
		}
	}
	else if (Stuff->IsA(ARunArrowStuff::StaticClass()))
	{
		Stuff->Destroy();
		auto player = Cast<AFitCharacter>(GetOwner());

		if (player)
		{
			auto controller = Cast<AFitPlayerController>(player->GetController());
			if (controller)
				controller->GyperRun();
		}
	}
}


