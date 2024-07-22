// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ChopRecipes.h"
#include "BlendRecipes.h"
#include "RecipeProvider.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class URecipeProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KITCHENGAME_API IRecipeProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UChopRecipes* GetChopRecipes();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UBlendRecipes* GetBlendRecipes();
};
