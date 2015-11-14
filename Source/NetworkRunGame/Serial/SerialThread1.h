#pragma once
#include "SerialClass.h"
#include "USerialA.h"

class NETWORKRUNGAME_API SerialThread1 : public FRunnable
{
	static SerialThread1* Runnable;

	static UUSerialA* BP;

	FString* theData;

	UUSerialA* TheBP;

	FRunnableThread* Thread;

	FThreadSafeCounter StopTaskCounter;

	static SerialClass *SP;

public:

	SerialThread1(UUSerialA* IN_BP);
	virtual ~SerialThread1();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	void EnsureCompletion();

	static SerialThread1* ThreadLauncher(UUSerialA* IN_BP, int32 portNum, int32 baudRate);

	static void Shutdown();

};
