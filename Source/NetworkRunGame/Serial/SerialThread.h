#pragma once
#include "SerialClass.h"
#include "SerialBPF.h"

class NETWORKRUNGAME_API SerialThread : public FRunnable
{
	static SerialThread* Runnable;

	static USerialBPF* BP;

	FString* theData;

	USerialBPF* TheBP;

	FRunnableThread* Thread;

	FThreadSafeCounter StopTaskCounter;

	static SerialClass *SP;

public:

	SerialThread(USerialBPF* IN_BP);
	virtual ~SerialThread();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	void EnsureCompletion();

	static SerialThread* ThreadLauncher(USerialBPF* IN_BP, int32 portNum, int32 baudRate);

	static void Shutdown();

};
