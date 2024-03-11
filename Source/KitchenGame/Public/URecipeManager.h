// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "URecipeManager.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FPresentationRecipe : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FPresentationRecipe()
    {}

    /** The stack of ingredients and utensils that should be on the presentation bench */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
    TArray<FString> ResultStack;

    /** How long the player will have to complete the recipe*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
    int TimeSeconds;
};

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

USTRUCT(BlueprintType)
struct FScoreDeduction
{
    GENERATED_USTRUCT_BODY()
    FScoreDeduction() : FScoreDeduction(0, TEXT("None"))
    {}

    FScoreDeduction(int ScoreDelta, const FString& Reason) : ScoreDelta(ScoreDelta), Reason(Reason)
    {}

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int ScoreDelta;

    /** */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Reason;
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

    UFUNCTION(BlueprintCallable, Category = Recipe)
    TArray<FScoreDeduction> TryPresent(const TArray<AActor*>& presented, const FPresentationRecipe& toMatch);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* ChopTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* BlendTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* PresentTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<AActor>> IngredientClasses;

private:

    bool MatchPresented(const AActor* presented, FString toMatch);

    void InitClassTable();

    UPROPERTY()
    TMap<FString, TSubclassOf<AActor>> NameToClass;
};
