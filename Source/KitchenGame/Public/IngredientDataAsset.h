// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IngredientDataAsset.generated.h"

class AIngredient;
enum class ESauceType : uint8;
class UPaperSprite;
/**
 * 
 */
UCLASS()
class KITCHENGAME_API UIngredientDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	UIngredientDataAsset();
public:
	/** The name of the ingredient */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText Name;

	/** The blueprint for the ingredient */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AIngredient> Ingredient;

	/** The Sprite used in some UI */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UPaperSprite> Sprite;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FLinearColor Color;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ESauceType SauceOutput;

	//Cooking Times

	/** How long it takes to cook */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	double CookTime;

	/** How long before it starts burning */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	double CookedTime;

	/** How long it takes to burn */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	double BurnTime;

	//Frying Times

	/** How long it takes to fry */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	double FryTime;

	/** How long before it starts burning */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	double FriedTime;

	/** How long it takes to over fry */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	double OverFryTime;

	/** How long it takes to burn */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool PreciseCooking;

	//Colors

	/** How long it takes to cook */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FLinearColor RawColor;

	/** How long before it starts burning */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FLinearColor CookColor;

	/** How long it takes to burn */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FLinearColor BurnColor;

	/** How long before it starts burning */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FLinearColor FryColor;

	/** How long it takes to burn */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FLinearColor OverFryColor;
};
