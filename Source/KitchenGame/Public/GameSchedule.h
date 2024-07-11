// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

USTRUCT(BlueprintType)
struct FConfiguredRecipe
{
	GENERATED_USTRUCT_BODY()
public:
	FConfiguredRecipe();

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<> Tutorial;

	UPROPERTY(BlueprintReadWrite)
	TArray<> Tutorial;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGameSchedule
{
	GENERATED_USTRUCT_BODY()
public:
	FGameSchedule();

	UPROPERTY(BlueprintReadWrite)
	bool Tutorial;

	UPROPERTY(BlueprintReadWrite)
	TArray<> Tutorial;
};
