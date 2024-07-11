// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeDataAsset.h"

URecipeDataAsset::URecipeDataAsset() :
	Name(),
	Widget(nullptr)
{
}

TArray<FScorePart> URecipeDataAsset::GetScoreFromStack(const TArray<AActor*>& presented)
{
	return TArray<FScorePart>();
}

UStackRecipeDataAsset::UStackRecipeDataAsset() : URecipeDataAsset(),
SauceModifierTargetIndex(-1),
CookPhaseModifierTargetIndex(-1)
{
}

TArray<FScorePart> UStackRecipeDataAsset::GetScoreFromStack(const TArray<AActor*>& presented)
{
	return TArray<FScorePart>();
}

UDrinkRecipeDataAsset::UDrinkRecipeDataAsset()
{
}

TArray<FScorePart> UDrinkRecipeDataAsset::GetScoreFromStack(const TArray<AActor*>& presented)
{
	return TArray<FScorePart>();
}
