// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "URecipeManager.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FCookingRecipe : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FCookingRecipe()
    {}

    /** Input ingredient ids for the recipe */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
    TArray<FString> RecipeInput;

    /** Output ingredient ids for the recipe */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
    TArray<FString> RecipeOutput;
};

UCLASS(Blueprintable, BlueprintType)
class KITCHENGAME_API URecipeManager : public UObject
{
	GENERATED_BODY()
public:
	URecipeManager();
	~URecipeManager();

    UFUNCTION(BlueprintCallable, Category = Recipe)
    void RegisterIngredient(FString ingredientId, TSubclassOf<AActor> ingredientClass);

    UFUNCTION(BlueprintCallable, Category = Recipe)
    TSubclassOf<AActor> TryChop(FString ingredientId);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* ChopTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* BlendTable;

private:
    UPROPERTY()
    TMap<FString, TSubclassOf<AActor>> IdToClass;
};
