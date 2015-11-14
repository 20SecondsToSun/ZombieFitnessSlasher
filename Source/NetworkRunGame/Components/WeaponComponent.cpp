// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "WeaponComponent.h"
#include "Player/FitCharacter.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	bReplicates = true;	
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...	
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, MainWeaponPatrons);
	DOREPLIFETIME(UWeaponComponent, AdditionalWeaponPatrons);
	DOREPLIFETIME(UWeaponComponent, WeaponType);
}

void UWeaponComponent::SetMainWeapon(UClass* WeaponClass, USkeletalMeshComponent* CharacterMesh)
{
	auto World = GetWorld();

	if (World && CharacterMesh)
	{
		MainWeapon = World->SpawnActor<AIWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		MainWeapon->IsAttached = true;
		MainWeapon->AttachRootComponentTo(CharacterMesh, AttachSocketName, EAttachLocation::SnapToTarget);
		CurrentWeapon = MainWeapon;	
		WeaponType = EWeaponCharacterEnum::MAIN;
	}
}

bool UWeaponComponent::CanFire()
{
	return CurrentWeapon != nullptr && (MainWeaponPatrons > 0 || AdditionalWeaponPatrons > 0);
}

void UWeaponComponent::Fire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire(Cast<AFitCharacter>(GetOwner()));
		RemoveCurrentWeaponPatrons();
	}	
}

void UWeaponComponent::AddMainWeaponPatrons()
{
	MainWeaponPatrons++;
}

void UWeaponComponent::AddMainWeaponPatrons_Implementation(int32 PatronsNum)
{
	MainWeaponPatrons += PatronsNum;
}

void UWeaponComponent::SetMainWeaponPatrons(int32 PatronsNum)
{
	MainWeaponPatrons = PatronsNum;
}

void UWeaponComponent::RemoveCurrentWeaponPatrons()
{
	if (WeaponType == EWeaponCharacterEnum::MAIN && MainWeaponPatrons > 0)	
	{
		MainWeaponPatrons--;
	}	
	else if (WeaponType == EWeaponCharacterEnum::ADDITIONAL && AdditionalWeaponPatrons > 0)
	{
		AdditionalWeaponPatrons--;

		if (AdditionalWeaponPatrons == 0)
		{	
			FTimerDelegate del = FTimerDelegate::CreateUObject(this, &UWeaponComponent::ResetMainWeaponHandler);
			GetOwner()->GetWorldTimerManager().SetTimer(ResetMainWeaponTimer, del, 1.0f, false);
		}
	}
}

void UWeaponComponent::ResetMainWeaponHandler()
{
	AdditionWeapon->Destroy();
	MainWeapon->SetActorHiddenInGame(false);
	WeaponType = EWeaponCharacterEnum::MAIN;
	CurrentWeapon = MainWeapon;
}

void UWeaponComponent::SetAdditionalWeapon(AIWeapon* _AdditionalWeapon)
{
	auto World = GetWorld();

	auto Character = Cast<AFitCharacter>(GetOwner());
	USkeletalMeshComponent* CharacterMesh = nullptr;
	if (Character)
		CharacterMesh = Character->GetMesh();

	if (_AdditionalWeapon && World && CharacterMesh)
	{	
		if (WeaponType == EWeaponCharacterEnum::ADDITIONAL && _AdditionalWeapon->ID == AdditionWeapon->ID)
		{
			AdditionalWeaponPatrons += _AdditionalWeapon->PatronsNum;
			_AdditionalWeapon->Destroy();
			return;
		}

		if (WeaponType == EWeaponCharacterEnum::ADDITIONAL)
			AdditionWeapon->Destroy();

		MainWeapon->SetActorHiddenInGame(true);

		WeaponType = EWeaponCharacterEnum::ADDITIONAL;
		AdditionalWeaponPatrons = _AdditionalWeapon->PatronsNum;
		AdditionWeapon = _AdditionalWeapon;
		AdditionWeapon->SetActorScale3D(FVector(1, 1, 1));

		//FString message = "=====Attach Weapon!!!!===";
		//UE_LOG(LogClass, Log, TEXT("%s"), *message);
		
		AdditionWeapon->AttachRootComponentTo(CharacterMesh, AttachSocketName, EAttachLocation::SnapToTarget);
		CurrentWeapon = AdditionWeapon;
	}	
}

int32 UWeaponComponent::GetCurrentPatronsNum()
{
	if (WeaponType == EWeaponCharacterEnum::MAIN)
	{
		return MainWeaponPatrons;
	}
	
	return AdditionalWeaponPatrons;	
}
