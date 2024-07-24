// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Blueprint/UserWidget.h"
#include "IngredientDataAsset.h"
#include "AIngredient.h"
#include "RecipeDataAsset.generated.h"

UENUM(BlueprintType)
enum class EStackRecipePosition : uint8
{
    Any,
    Top,
    Bottom
};

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

USTRUCT(BlueprintType)
struct FRecipeScore
{
    GENERATED_USTRUCT_BODY()
public:
    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<URecipeDataAsset> Recipe;

    /** */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FScorePart> Parts;
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
	TSubclassOf<UUserWidget> Widget;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    float Time;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    int IngredientCount;


    UFUNCTION(BlueprintCallable, BlueprintPure)
    virtual bool CanSubmitStack(const TArray<AActor*>& presented);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    virtual TArray<FScorePart> GetScoreFromStack(const TArray<AActor*>& presented, ESecondaryCookPhase ModifierCookPhase, ESauceType ModifierSauceType);

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
    ESauceType Sauce;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    ESecondaryCookPhase SecondaryCookPhase;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    TArray<FRecipeItem> Items;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    bool RequirePlate;

    virtual bool CanSubmitStack(const TArray<AActor*>& presented) override;

    virtual TArray<FScorePart> GetScoreFromStack(const TArray<AActor*>& presented, ESecondaryCookPhase ModifierCookPhase, ESauceType ModifierSauceType) override;

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
    TArray<TObjectPtr<UIngredientDataAsset>> Ingredients;

    virtual bool CanSubmitStack(const TArray<AActor*>& presented) override;

    virtual TArray<FScorePart> GetScoreFromStack(const TArray<AActor*>& presented, ESecondaryCookPhase ModifierCookPhase, ESauceType ModifierSauceType) override;

};