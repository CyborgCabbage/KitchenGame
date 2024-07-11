// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Blueprint/UserWidget.h"
#include "IngredientDataAsset.h"
#include "AIngredient.h"
#include "RecipeDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FScorePart
{
    GENERATED_USTRUCT_BODY()
public:
    FScorePart() : FScorePart(0, FText::FromString("None"))
    {}

    FScorePart(int Value, const FText& Reason) : Value(Value), Reason(Reason)
    {}

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Value;

    /** */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Reason;
};

/**
 * 
 */
UCLASS(Abstract)
class KITCHENGAME_API URecipeDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
    URecipeDataAsset();

	/** The name of the recipe (for the stats screen) */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UUserWidget> Widget;

    UFUNCTION(BlueprintCallable)
    virtual TArray<FScorePart> GetScoreFromStack(const TArray<AActor*>& presented);

};

UENUM(BlueprintType)
enum class EStackRecipePosition : uint8
{
    Any,
    Top,
    Bottom
};

USTRUCT(BlueprintType)
struct FRecipeItem
{
    GENERATED_USTRUCT_BODY()
public:

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UIngredientDataAsset> Ingredient;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPrimaryCookPhase CookPhase;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStackRecipePosition Position;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESauceType Sauce;
};

/**
 *
 */
UCLASS()
class KITCHENGAME_API UStackRecipeDataAsset : public URecipeDataAsset
{
    GENERATED_BODY()
public:
    UStackRecipeDataAsset();

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    int SauceModifierTargetIndex;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    int CookPhaseModifierTargetIndex;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TArray<FRecipeItem> Items;

    virtual TArray<FScorePart> GetScoreFromStack(const TArray<AActor*>& presented) override;

};

/**
 *
 */
UCLASS()
class KITCHENGAME_API UDrinkRecipeDataAsset : public URecipeDataAsset
{
    GENERATED_BODY()
public:
    UDrinkRecipeDataAsset();

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TArray<AActor> Items;

    virtual TArray<FScorePart> GetScoreFromStack(const TArray<AActor*>& presented) override;

};