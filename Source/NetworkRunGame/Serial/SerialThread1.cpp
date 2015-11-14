// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "SerialThread1.h"
#include "USerialA.h"
#include <string>

SerialThread1* SerialThread1::Runnable = NULL;

SerialClass *SerialThread1::SP = new SerialClass();

UUSerialA *SerialThread1::BP;// = NewObject<UThreadBP>();

char incomingData[1] = {};			// don't forget to pre-allocate memory
char outData[250] = {};
char writeBuffer[250] = {};
int len = 0;
int32 thePort;
int32 theBaud;

SerialThread1::SerialThread1(UUSerialA* IN_BP) : TheBP(IN_BP), StopTaskCounter(0)
{
	Thread = FRunnableThread::Create(this, TEXT("ThreadTest"), 0, TPri_BelowNormal);
}

SerialThread1::~SerialThread1()
{
	delete Thread;
	Thread = NULL;
}

bool SerialThread1::Init()
{
	// TODO: Initialization

	return true;
}

uint32 SerialThread1::Run()
{
	FPlatformProcess::Sleep(0.03);
	while (StopTaskCounter.GetValue() == 0 && SP->IsOpened())
	{

		int nBytesRead = SP->ReadData(incomingData, 1);
		if (nBytesRead > 0)
		{
			if (len > 180 || incomingData[0] == '\n')
			{
				if (strlen(outData) > 0)
				{
					std::string test(outData);
					FString Fout(test.c_str());
					TheBP->GetData(Fout);
					len = 0;
					memset(outData, 0, sizeof outData);
				}
			}
			else
			{
				outData[len] = incomingData[0];
				len++;
			}
		}
		FString writeData = TheBP->SendData();
		strcpy_s(writeBuffer, TCHAR_TO_ANSI(*writeData));
		SP->SendData(writeBuffer, strlen(writeBuffer));
	}
	return 0;
}

void SerialThread1::Stop()
{
	StopTaskCounter.Increment();
}

SerialThread1* SerialThread1::ThreadLauncher(UUSerialA* IN_BP, int32 portNum, int32 baudRate)
{
	thePort = portNum;
	theBaud = baudRate;

	BP = NewObject<UUSerialA>();

	if (SP->Open(thePort, theBaud))
	{
		BP->SerialOpened(true);
	}
	else
	{
		BP->SerialOpened(false);
	}
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{

		Runnable = new SerialThread1(IN_BP);
	}

	return Runnable;
}

void SerialThread1::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void SerialThread1::Shutdown()
{

	if (SP->Close())
	{
		BP->IsSerialClosed(true);
	}
	else
	{
		BP->IsSerialClosed(false);
	}
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
	if (Runnable == NULL)
	{
		BP->IsThreadClosed(true);
	}
	else
	{
		BP->IsThreadClosed(false);
	}
}
