// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "USerialA.h"
#include "Serial/SerialThread1.h"

SerialClass *UUSerialA::SP;// = new CSerial();
UUSerialA *UUSerialA::BP;// = NewObject<UThreadBP>();
FString UUSerialA::returnIt;
FString UUSerialA::sendIt;
FString UUSerialA::testStr;
bool UUSerialA::serialShutdown;
bool UUSerialA::threadShutdown;
bool UUSerialA::threadIsOpen;
bool UUSerialA::serialIsOpen;

UUSerialA::UUSerialA(const class FObjectInitializer& PCIP) : Super(PCIP)
{

}

void UUSerialA::OpenSerial(int32 portNum, int32 baudRate)
{
	SP = new SerialClass();
	BP = NewObject<UUSerialA>();
	BP->StartThread(portNum, baudRate);
}

void UUSerialA::CloseSerial()
{
	SerialThread1::Shutdown();

	if (threadShutdown)
	{
		UE_LOG(LogTemp, Log, TEXT("THREAD CLOSED!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: THREAD NOT CLOSED!!"));
	}

	if (serialShutdown)
	{
		UE_LOG(LogTemp, Log, TEXT("SERIAL PORT CLOSED!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: SERIAL PORT NOT CLOSED!!"));
	}
}

FString UUSerialA::GetData(FString data)
{
	returnIt = data.TrimTrailing();
	return data;
}

FString UUSerialA::ReadSerial()
{
	return returnIt;
}

void UUSerialA::WriteSerial(FString writeData)
{
	sendIt = writeData;
}

FString UUSerialA::SendData()
{
	FString temp = sendIt;
	sendIt = "";
	return temp;
}

void UUSerialA::StartThread(int32 portNum, int32 baudRate)
{
	if (SerialThread1::ThreadLauncher(this, portNum, baudRate) != NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("THREAD STARTED!!"));
		threadIsOpen = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: THREAD NOT STARTED!!"));
		threadIsOpen = false;
	}

	if (serialIsOpen)
	{
		UE_LOG(LogTemp, Log, TEXT("SERIAL PORT OPENED!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: SERIAL PORT NOT OPENED!!"));
	}
}

bool UUSerialA::IsSerialPortOpen()
{
	return serialIsOpen && threadIsOpen;
}

void UUSerialA::IsThreadClosed(bool threadClosed)
{
	threadShutdown = threadClosed;
}

void UUSerialA::IsSerialClosed(bool serialClosed)
{
	serialShutdown = serialClosed;
}

void UUSerialA::SerialOpened(bool serialOpen)
{
	serialIsOpen = serialOpen;
}





