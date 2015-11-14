// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "AimComponent.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UAimComponent::UAimComponent()
{
	bReplicates = true;	
	//bAlwaysRelevant = true;
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UAimComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAimComponent, IsAlive);
	DOREPLIFETIME(UAimComponent, OnFire);
	//DOREPLIFETIME(UAimComponent, SeeAngle);
	DOREPLIFETIME(UAimComponent, DistanceTo);
	DOREPLIFETIME(UAimComponent, SceneLinkedComponent);
}

void UAimComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAimComponent::SetColorHighlight_Implementation()
{	
	for (size_t i = 0; i < MaterialDynamicInstances.Num(); i++)
	{
		if (MaterialDynamicInstances[i])
			MaterialDynamicInstances[i]->SetVectorParameterValue(FName("EmissiveMask"), FLinearColor(0.4f, 0.0f, 0.0f, 1.0f));
	}	
}

void UAimComponent::SetDefaultHighlight_Implementation()
{		
	for (size_t i = 0; i < MaterialDynamicInstances.Num(); i++)
	{
		if (MaterialDynamicInstances[i])
			MaterialDynamicInstances[i]->SetVectorParameterValue(FName("EmissiveMask"), FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	}
}

void UAimComponent::SetEnemyAimOnFire_Implementation()
{
	OnFire = true;

	FTimerHandle _Timer;
	FTimerDelegate del = FTimerDelegate::CreateUObject(this, &UAimComponent::ResetOnFire);
	GetOwner()->GetWorldTimerManager().SetTimer(_Timer, del, 2.0f, false);
}

//void UAimComponent::CalculateAngle_Implementation(AActor* CHaracterActor, AActor* AimActor)
//{
//	auto CharacterNormForward = CHaracterActor->GetActorForwardVector();
//	CharacterNormForward.Normalize();
//
//	auto TargetLocation = AimActor->GetActorLocation();
//
//	auto VecvtorToTarget = TargetLocation - CHaracterActor->GetActorLocation();
//	VecvtorToTarget.Normalize();
//
//	auto frwdVec = CHaracterActor->GetActorLocation() + 500 * CharacterNormForward;
//	auto toTrgtVec = CHaracterActor->GetActorLocation() + 500 * VecvtorToTarget;
//	toTrgtVec.Z = frwdVec.Z = 0;
//
//	frwdVec = CharacterNormForward;
//	toTrgtVec = VecvtorToTarget;
//
//	auto dot = toTrgtVec.X * frwdVec.X + toTrgtVec.Y * frwdVec.Y;//  # dot product
//	auto det = toTrgtVec.X * frwdVec.Y - toTrgtVec.Y * frwdVec.X;//  # determinant
//	auto radians = FMath::Atan2(det, dot); // # atan2(y, x) or atan2(sin, cos)
//	auto degrees = FMath::RadiansToDegrees(radians);
//
//	SeeAngle = FMath::Abs(degrees);
//}

void UAimComponent::SetDistance_Implementation(float Distance)
{
	DistanceTo = Distance;
}

void UAimComponent::ResetOnFire()
{
	if (IsAlive)
	{
		OnFire = false;
	}
}

bool UAimComponent::CanBeAimed()
{
	return IsAlive && !OnFire;
}

bool UAimComponent::IsClose()
{
	return DistanceTo > 0 && DistanceTo < 6000.0f;
}

void UAimComponent::SetSceneLinkedComponent_Implementation(USceneComponent* value)
{
	SceneLinkedComponent = value;
}

USceneComponent* UAimComponent::GetSceneLinkedComponent() 
{ 
	return SceneLinkedComponent;
}
