// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class NETWORKRUNGAME_API FitTypes
{
public:
	FitTypes();
	~FitTypes();
};

UENUM(BlueprintType)
enum class EGraphicInterfaceEnum : uint8
{
	VE_NONE 	UMETA(DisplayName = "None"),
	VE_RUN_RUN 	UMETA(DisplayName = "Run"),
	VE_DAMAGE 	UMETA(DisplayName = "Damage"),
	VE_FREEZE	UMETA(DisplayName = "Freeze"),
	VE_GUN		UMETA(DisplayName = "Gun")
};


UENUM(BlueprintType)
enum class EWeaponCharacterEnum : uint8
{
	MAIN 	UMETA(DisplayName = "MAIN"),
	ADDITIONAL 	UMETA(DisplayName = "ADDITIONAL")
};
