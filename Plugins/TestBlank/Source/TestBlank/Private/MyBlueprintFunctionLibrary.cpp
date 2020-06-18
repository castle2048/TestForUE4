// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

#if PLATFORM_ANDROID
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/Launch/Public/Android/AndroidEventManager.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#endif
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

void UMyBlueprintFunctionLibrary::TestCallJarFun()
{
	UE_LOG(LogTemp, Log, TEXT("error: cc UMyBlueprintFunctionLibrary::TestCallJarFun"));
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		UE_LOG(LogTemp, Log, TEXT("error: cc FAndroidApplication::GetJavaEnv"));
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_TestCallJar", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}

#endif
}


void UMyBlueprintFunctionLibrary::Generic_AcessArrayByName(UObject* OwnerObject, FName ArrayPropertyName, void* SrcArrayAddr, const UArrayProperty* SrcArrayProperty, bool bSetter /*= true*/)
{
	if (OwnerObject != NULL)
	{
		UArrayProperty* ArrayProp = FindField<UArrayProperty>(OwnerObject->GetClass(), ArrayPropertyName);

		if (ArrayProp != NULL && (ArrayProp->SameType(SrcArrayProperty)))
		{
			void* Dest = ArrayProp->ContainerPtrToValuePtr<void>(OwnerObject);
			if (bSetter == true)
			{
				ArrayProp->CopyValuesInternal(Dest, SrcArrayAddr, 1);
			}
			else
			{
				ArrayProp->CopyValuesInternal(SrcArrayAddr, Dest, 1);
			}
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UParserPropertyLibrary::Generic_AcessArrayByName: Failed to find %s array from %s object"), *ArrayPropertyName.ToString(), *UKismetSystemLibrary::GetDisplayName(OwnerObject));
}

void UMyBlueprintFunctionLibrary::SortStructArray(UPARAM(ref)TArray<FSortStruct>& TargetArray, TArray<FSortStruct>& Array, const bool bAscending)
{
	if (bAscending)
	{
		TargetArray.Sort([](const FSortStruct& a, const FSortStruct& b) {return a.ComparedVar < b.ComparedVar; });
	}
	else
	{
		TargetArray.Sort([](const FSortStruct& a, const FSortStruct& b) {return a.ComparedVar > b.ComparedVar; });
	}
	Array = TargetArray;
	return;
}

void UMyBlueprintFunctionLibrary::Generic_StortUserDefinedStructArray(void* TargetArray, const UArrayProperty* ArrayProp, UObject* OwnerObject, UFunction* SortRuleFunc)
{
	if (!SortRuleFunc || !OwnerObject || !TargetArray)
	{
		return;
	}
	UBoolProperty* ReturnParam = Cast<UBoolProperty>(SortRuleFunc->GetReturnProperty());
	if (!ReturnParam)
	{
		return;
	}
	//Begin Sort Array
	FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
	UProperty* InnerProp = ArrayProp->Inner;

	const int32 Len = ArrayHelper.Num();
	const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;

	uint8* Parameters = (uint8*)FMemory::Malloc(PropertySize * 2 + 1);

	for (int32 i = 0; i < Len; i++)
	{
		for (int32 j = 0; j < Len - i - 1; j++)
		{
			FMemory::Memzero(Parameters, PropertySize * 2 + 1);
			InnerProp->CopyCompleteValueFromScriptVM(Parameters, ArrayHelper.GetRawPtr(j));
			InnerProp->CopyCompleteValueFromScriptVM(Parameters + PropertySize, ArrayHelper.GetRawPtr(j + 1));
			OwnerObject->ProcessEvent(SortRuleFunc, Parameters);
			if (ReturnParam && ReturnParam->GetPropertyValue(Parameters + PropertySize * 2))
			{
				ArrayHelper.SwapValues(j, j + 1);
			}
		}

	}
	FMemory::Free(Parameters);
	// end sort array
}

void UMyBlueprintFunctionLibrary::Generic_ShowStructFields(const void* StructAddr, const UStructProperty* StructProperty)
{
	UScriptStruct* Struct = StructProperty->Struct;
	for (TFieldIterator<UProperty> iter(Struct); iter; ++iter)
	{
		FScreenMessageString NewMessage;
		NewMessage.CurrentTimeDisplayed = 0.0f;
		NewMessage.Key = INDEX_NONE;
		NewMessage.DisplayColor = FColor::Blue;
		NewMessage.TimeToDisplay = 5;
		NewMessage.ScreenMessage = FString::Printf(TEXT("Property: [%s].[%s]"), *(Struct->GetName()), *(iter->GetName()));
		NewMessage.TextScale = FVector2D::UnitVector;

		GEngine->PriorityScreenMessages.Insert(NewMessage, 0);
	}
}

float UMyBlueprintFunctionLibrary::Generic_Array_NumericPropertyAverage(const void* TargetArray, const UArrayProperty* ArrayProperty, FName ArrayPropertyName)
{
	UStructProperty* InnerProperty = Cast<UStructProperty>(ArrayProperty->Inner);
	if (!InnerProperty)
	{
		UE_LOG(LogTemp, Log, TEXT("error: cc Array inner property is not a ustruct!"));
		return 0.f;
	}

	UScriptStruct* Struct = InnerProperty->Struct;
	FString PropertyNameStr = ArrayPropertyName.ToString();
	UNumericProperty* NumProperty = nullptr;
	for (TFieldIterator<UNumericProperty> iter(Struct); iter; ++iter)
	{
		if (Struct->PropertyNameToDisplayName(iter->GetFName()) == PropertyNameStr)
		{
			NumProperty = *iter;
			break;
		}
	}
	if (!NumProperty)
	{
		UE_LOG(LogTemp, Log, TEXT("error: cc Strcut property not numeric = [%s]"), *(PropertyNameStr));
	}

	FScriptArrayHelper ArrayHelper(ArrayProperty, TargetArray);
	int Count = ArrayHelper.Num();
	float Sum = 0.f;

	if (Count <= 0)
	{
		return 0.f;
	}

	if (NumProperty->IsFloatingPoint())
	{
		for (int i = 0; i< Count; i++)
		{
			void* ElemPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElemPtr);
			Sum += NumProperty->GetFloatingPointPropertyValue(ValuePtr);
		}
	}
	else if(NumProperty->IsInteger())
	{
		for (int i = 0; i < Count; i++)
		{
			void* ElemPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElemPtr);
			Sum += NumProperty->GetSignedIntPropertyValue(ValuePtr);
		}
	}

	return Sum / Count;
}
