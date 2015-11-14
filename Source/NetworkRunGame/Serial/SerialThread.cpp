// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkRunGame.h"
#include "SerialThread.h"
#include "SerialBPF.h"
#include <string>
SerialThread* SerialThread::Runnable = NULL;

SerialClass *SerialThread::SP = new SerialClass();

USerialBPF *SerialThread::BP;// = NewObject<UThreadBP>();

char incomingData1[1] = {};			// don't forget to pre-allocate memory
char outData1[250] = {};
char writeBuffer1[250] = {};
int len1 = 0;
int32 thePort1;
int32 theBaud1;

SerialThread::SerialThread(USerialBPF* IN_BP) : TheBP(IN_BP), StopTaskCounter(0)
{
	Thread = FRunnableThread::Create(this, TEXT("ThreadTest"), 0, TPri_BelowNormal);
}

SerialThread::~SerialThread()
{
	delete Thread;
	Thread = NULL;
}

bool SerialThread::Init()
{
	// TODO: Initialization

	return true;
}

uint32 SerialThread::Run()
{
	FPlatformProcess::Sleep(0.03);
	while (StopTaskCounter.GetValue() == 0 && SP->IsOpened())
	{
		int nBytesRead = SP->ReadData(incomingData1, 1);
		if (nBytesRead > 0)
		{
			if (len1 > 180 || incomingData1[0] == '\n')//strcmp(incomingData1, "\r") == 0 || strcmp(incomingData1, "\n") == 0 || strcmp(incomingData1, "\t") == 0 || len1 > 180)
			{
				if (strlen(outData1) > 0)
				{
					std::string test(outData1);
					FString Fout(test.c_str());
					TheBP->GetData(Fout);
					len1 = 0;
					memset(outData1, 0, sizeof outData1);
				}
			}
			else
			{
				//bool flag = false;

				//if (len1 == 0)
				//{
				//	flag = (incomingData1[0] == 's');//(strcmp(incomingData1, "s") == 0);
				//}
				//else if (len1 == 1)
				//{
				//	flag = (incomingData1[0] == 't');// (strcmp(incomingData1, "t") == 0);
				//}
				//else if (len1 == 2)
				//{
				//	flag = (incomingData1[0] == 'e');// (strcmp(incomingData1, "e") == 0);
				//}
				//else
				//	flag = true;

				//if (flag)
				//{
					outData1[len1] = incomingData1[0];
					len1++;
				//}
			
			}
		}

		FString writeData = TheBP->SendData();
		strcpy_s(writeBuffer1, TCHAR_TO_ANSI(*writeData));
		SP->SendData(writeBuffer1, strlen(writeBuffer1));
	}
	return 0;
}

void SerialThread::Stop()
{
	StopTaskCounter.Increment();
}

SerialThread* SerialThread::ThreadLauncher(USerialBPF* IN_BP, int32 portNum, int32 baudRate)
{
	thePort1 = portNum;
	theBaud1 = baudRate;

	BP = NewObject<USerialBPF>();

	if (SP->Open(thePort1, theBaud1))
	{
		BP->SerialOpened(true);
	}
	else
	{
		BP->SerialOpened(false);
	}
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{

		Runnable = new SerialThread(IN_BP);
	}

	return Runnable;
}

void SerialThread::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void SerialThread::Shutdown()
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
