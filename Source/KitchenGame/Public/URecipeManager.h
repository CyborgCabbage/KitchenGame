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

USTRUCT(BlueprintType)
struct FTryRecipeResult
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool Success;

    /** */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<TSubclassOf<AActor>> ToCreate;

    /** */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<AActor*> ToDestroy;
};

UCLASS(Blueprintable, BlueprintType)
class KITCHENGAME_API URecipeManager : public UObject
{
	GENERATED_BODY()
public:
	URecipeManager();
	~URecipeManager();

    UFUNCTION(BlueprintCallable, Category = Recipe)
    FTryRecipeResult TryChop(AActor* ingredient);

    UFUNCTION(BlueprintCallable, Category = Recipe)
    FTryRecipeResult TryBlend(const TArray<AActor*>& ingredients);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* ChopTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* BlendTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<AActor>> IngredientClasses;

private:
    void InitClassTable();

    UPROPERTY()
    TMap<FString, TSubclassOf<AActor>> NameToClass;
};
