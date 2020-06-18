// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../ThirdParty/Hello/Include/Share.h"
#include "../../ThirdParty/Hello/Include/Hello.h"
#include "MyActor.generated.h"

UCLASS()
class TESTBLANK_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Test1")
		FString CallLib(const FString msg);

	UFUNCTION(BlueprintCallable, Category = "Test1")
		FString CallDll(const FString msg);


};
