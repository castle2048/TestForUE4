// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestBlueprintLibraryBPLibrary.h"
#include "TestBlueprintLibrary.h"

UTestBlueprintLibraryBPLibrary::UTestBlueprintLibraryBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UTestBlueprintLibraryBPLibrary::TestBlueprintLibrarySampleFunction(float Param)
{
	return -1;
}

