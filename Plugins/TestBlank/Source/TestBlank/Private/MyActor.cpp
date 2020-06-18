// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "../../ThirdParty/Hello/Include/MyLibTest.h"

//#define PLATFORM_WINDOWS 1


#ifdef PLATFORM_WINDOWS
typedef float(*_dllfun)(float raduis);
#else
typedef std::string(*_dllfun)(std::string msg);
#endif // PLATFORM_WINDOWS

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AMyActor::CallLib(const FString msg)
{
#if PLATFORM_WINDOWS
	UE_LOG(LogTemp, Log, TEXT("error: cc CallLib r = %s"), *msg);

	MyLIB *myLib = new MyLIB();
	float ret = myLib->GetArea(FCString::Atof(*msg));

	return FString::SanitizeFloat(ret);
#endif
	return "None";
}

FString AMyActor::CallDll(const FString msg)
{
#ifdef PLATFORM_WINDOWS
	FString fileP = "TestBlank/ThirdParty/Hello/Lib/HelloDLL2.dll";
#else
	FString fileP = "TestBlank/ThirdParty/Hello/Lib/HelloDLL.dll";
#endif 

	FString filePath = FPaths::Combine(*FPaths::ProjectPluginsDir(), fileP);
	if (FPaths::FileExists(filePath))
	{
		UE_LOG(LogTemp, Log, TEXT("error: cc Dll is exists filePath = %s"), *filePath);
		void* DLLHandle;
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath);
		if (DLLHandle != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("error: cc DllHandle is vaild"));
			_dllfun DllFun = NULL;
#ifdef PLATFORM_WINDOWS
			FString getArea = "getCircleArea";
			DllFun = (_dllfun)FPlatformProcess::GetDllExport(DLLHandle, *getArea);
#else
			FString sayFun = "sayHello";
			DllFun = (_dllfun)FPlatformProcess::GetDllExport(DLLHandle, *sayFun);
#endif 
			
			
			if (DllFun != NULL)
			{
				UE_LOG(LogTemp, Log, TEXT("error: cc SayFun is vaild"));
#ifdef PLATFORM_WINDOWS
				float ret = DllFun(2.0);
				return FString::SanitizeFloat(ret);
#else
				std::string ms(TCHAR_TO_UTF8(*msg));
				std::string ret = DllFun(ms);
				return ret.c_str();
#endif // PLATFORM_WINDOWS
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("error: cc SayFun is not vaild"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("error: cc DllHandle is not vaild"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("error: cc Dll not found"));
	}
	return "None";
}

