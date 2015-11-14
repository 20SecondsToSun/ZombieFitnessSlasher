// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "SerialBPF.h"

#include "Serial/SerialThread.h"

SerialClass *USerialBPF::SP;// = new CSerial();
USerialBPF *USerialBPF::BP;// = NewObject<UThreadBP>();
FString USerialBPF::returnIt;
FString USerialBPF::sendIt;
FString USerialBPF::testStr;
bool USerialBPF::serialShutdown;
bool USerialBPF::threadShutdown;
bool USerialBPF::threadIsOpen;
bool USerialBPF::serialIsOpen;

USerialBPF::USerialBPF(const class FObjectInitializer& PCIP) : Super(PCIP)
{

}

void USerialBPF::OpenSerial(int32 portNum, int32 baudRate)
{
	SP = new SerialClass();
	BP = NewObject<USerialBPF>();
	BP->StartThread(portNum, baudRate);
}

void USerialBPF::CloseSerial()
{
	SerialThread::Shutdown();

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

FString USerialBPF::GetData(FString data)
{
	returnIt = data.TrimTrailing();
	return data;
}

FString USerialBPF::ReadSerial()
{
	return returnIt;
}

void USerialBPF::WriteSerial(FString writeData)
{
	sendIt = writeData;
}

FString USerialBPF::SendData()
{
	FString temp = sendIt;
	sendIt = "";
	return temp;
}

void USerialBPF::StartThread(int32 portNum, int32 baudRate)
{
	if (SerialThread::ThreadLauncher(this, portNum, baudRate) != NULL)
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

bool USerialBPF::IsSerialPortOpen()
{
	return serialIsOpen && threadIsOpen;
}

void USerialBPF::IsThreadClosed(bool threadClosed)
{
	threadShutdown = threadClosed;
}

void USerialBPF::IsSerialClosed(bool serialClosed)
{
	serialShutdown = serialClosed;
}

void USerialBPF::SerialOpened(bool serialOpen)
{
	serialIsOpen = serialOpen;
}



