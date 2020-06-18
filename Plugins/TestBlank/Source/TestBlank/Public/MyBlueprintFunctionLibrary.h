// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

#define P_GET_GENERIC_ARRAY(ArrayAddr, ArrayProperty) Stack.MostRecentProperty = nullptr;\
	Stack.StepCompiledIn<UArrayProperty>(NULL);\
	void* ArrayAddr = Stack.MostRecentPropertyAddress;\
	UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);\
	if (!ArrayProperty)	{Stack.bArrayContextFailed = true; return;}

USTRUCT(BlueprintType)
struct FSortStruct
{
	GENERATED_BODY()

	FSortStruct() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ComparedVar;

	FSortStruct(int a)
		: ComparedVar(a)
	{
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct FDummyStruct
{
	GENERATED_USTRUCT_BODY()
};

/**
 * 
 */
UCLASS()
class TESTBLANK_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = TryJAR)
		static void TestCallJarFun();

	//---------------------------------------泛型蓝图节点相关--------------------------------------------------------------------------
	// 蓝图中调用不到函数
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "Variable", AutoCreateRefTerm = "Variable"), Category = TryJAR)
		static void FunctionName(const int32& Variable);
	DECLARE_FUNCTION(execFunctionName)
	{

	}

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Utilities|Array", meta = (ArrayParm = "Value", ArrayTypeDependentParams = "Value", DisplayName = "GET/SET (array)", CompactNodeTitle = "GET/SET"))
		static void AccessArrayByName(UObject* Object, FName PropertyName, const TArray<int32>& Value, bool bSetter = true);
	// get array
	DECLARE_FUNCTION(execAccessArrayByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(UNameProperty, ArrayPropertyName);

		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* SrcArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* SrcArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);

		P_GET_UBOOL(bSetter);
		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_AcessArrayByName(OwnerObject, ArrayPropertyName, SrcArrayAddr, SrcArrayProperty, bSetter);
		P_NATIVE_END;
	}

	// Get or set a ARRAY Property of UObject by property name
	static void Generic_AcessArrayByName(UObject* OwnerObject, FName ArrayPropertyName, void* SrcArrayAddr, const UArrayProperty* SrcArrayProperty, bool bSetter = true);

	//-------------------------------------------实现任意类型数组蓝图排序节点----------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Algorithm|Utilities|Array", meta = (Keywords = "sort, array, struct", DisplayName = "Sort (Struct)"))
		static void SortStructArray(UPARAM(ref) TArray<FSortStruct>& TargetArray, TArray<FSortStruct>& Array, const bool bAscending = true);

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "sort", DefaultToSelf = "Object", ArrayParm = "CustomStruct", AdvancedDisplay = "Object"), Category = "Utilities|UserDefinedStruct")
		static void StortUserDefinedStructArray(const TArray<int32>& CustomStruct, UObject* Object, FName FunctionName);

	static void Generic_StortUserDefinedStructArray(void* TargetArray, const UArrayProperty* ArrayProp, UObject* OwnerObject, UFunction* SortRuleFunc);

	DECLARE_FUNCTION(execStortUserDefinedStructArray)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* ArrayAAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(UNameProperty, FunctionName);
		if (!OwnerObject)
		{
			return;
		}
		UFunction* const Func = OwnerObject->FindFunction(FunctionName);
		if ((!Func || (Func->NumParms != 3)))
		{
			return;
		}
		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_StortUserDefinedStructArray(ArrayAAddr, ArrayProperty, OwnerObject, Func);
		P_NATIVE_END;
	}
	//-------------------------------------------实现泛型节点----------------------------------------------------------------------
	UFUNCTION(BlueprintCallable, CustomThunk, Category = TryJAR, meta = (CustomStructureParam = "CustomStruct"))
		static void ShowStructFields(const FDummyStruct& CustomStruct);
	static void Generic_ShowStructFields(const void* StructAddr, const UStructProperty* StructProperty);

	DECLARE_FUNCTION(execShowStructFields)
	{
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		UStructProperty* StructProperty = Cast<UStructProperty>(Stack.MostRecentProperty);
		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_ShowStructFields(StructAddr, StructProperty);
		P_NATIVE_END;
	}

	UFUNCTION(BlueprintPure, CustomThunk, meta = (DisplayName = "ArrayNumericPropertyAverage", ArrayParm = "TargetArray", ArrayTypeDependentParams = "TargetArray"), Category = TryJAR)
		static float Array_NumericPropertyAverage(const TArray<int32>& TargetArray, FName ArrayPropertyName);
	static float Generic_Array_NumericPropertyAverage(const void* TargetArray, const UArrayProperty* ArrayProperty, FName ArrayPropertyName);

	DECLARE_FUNCTION(execArray_NumericPropertyAverage)
	{
		P_GET_GENERIC_ARRAY(ArrayAddr, ArrayProperty);

		P_GET_PROPERTY(UNameProperty, PropertyName);

		P_FINISH;

		P_NATIVE_BEGIN;
		*(float*)RESULT_PARAM = Generic_Array_NumericPropertyAverage(ArrayAddr, ArrayProperty, PropertyName);
		P_NATIVE_END;
	}

};