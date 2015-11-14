// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SerialClass.h"
#include "USerialA.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKRUNGAME_API UUSerialA : public UObject
{
	GENERATED_BODY()

private:
	static SerialClass* SP;
	static UUSerialA* BP;

	static FString returnIt;
	static FString sendIt;
	static FString testStr;
	static bool serialShutdown;
	static bool threadShutdown;
	static bool threadIsOpen;
	static bool serialIsOpen;


public:
	UUSerialA(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static void OpenSerial(int32 portNum, int32 baudRate);

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static void CloseSerial();

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static FString ReadSerial();

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static void WriteSerial(FString writeData);

	UFUNCTION(BlueprintCallable, Category = "UE4Duino")
		static bool IsSerialPortOpen();


	void StartThread(int32 portNum, int32 baudRate);
	FString GetData(FString data);
	FString SendData();
	void IsThreadClosed(bool threadClosed);
	void IsSerialClosed(bool serialClosed);
	void SerialOpened(bool serialOpen);
	
	
	
	
};
