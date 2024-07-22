// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IngredientDataAsset.h"
#include "BlendRecipes.generated.h"

USTRUCT(BlueprintType)
struct FBlendRecipe {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<UIngredientDataAsset>> Input;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UIngredientDataAsset> Output;
};

/**
 *
 */
UCLASS(BlueprintType)
class KITCHENGAME_API UBlendRecipes : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBlendRecipe> BlendRecipes;

	UFUNCTION(BlueprintCallable)
	bool CheckRecipe(const TArray<AActor*> InputActors, TSubclassOf<AActor>& OutputClass);
};
