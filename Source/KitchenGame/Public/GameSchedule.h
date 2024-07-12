// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecipeDataAsset.h"
#include "GameSchedule.generated.h"

USTRUCT(BlueprintType)
struct FConfiguredRecipe
{
	GENERATED_USTRUCT_BODY()
public:
	FConfiguredRecipe();

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<URecipeDataAsset> RecipeAsset;

	UPROPERTY(BlueprintReadWrite)
	ESecondaryCookPhase ModifierCookPhase;

	UPROPERTY(BlueprintReadWrite)
	ESauceType ModifierSauceType;

	UPROPERTY(BlueprintReadWrite)
	bool Tutorial;
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
	TArray<FConfiguredRecipe> Recipes;
};
