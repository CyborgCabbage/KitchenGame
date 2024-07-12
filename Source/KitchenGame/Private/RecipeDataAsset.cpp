// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeDataAsset.h"

URecipeDataAsset::URecipeDataAsset() :
	Name(),
	Time(60.0f),
	Widget(nullptr)
{
}

bool URecipeDataAsset::CanSubmitStack(const TArray<AActor*>& presented)
{
	return false;
}

TArray<FScorePart> URecipeDataAsset::GetScoreFromStack(const TArray<AActor*>& presented, ESecondaryCookPhase ModifierCookPhase, ESauceType ModifierSauceType)
{
	return TArray<FScorePart>();
}

UStackRecipeDataAsset::UStackRecipeDataAsset() : URecipeDataAsset(),
	Sauce(ESauceType::None),
	SecondaryCookPhase(ESecondaryCookPhase::None)
{
}

bool UStackRecipeDataAsset::CanSubmitStack(const TArray<AActor*>& presented)
{
	return true;
}

TArray<FScorePart> UStackRecipeDataAsset::GetScoreFromStack(const TArray<AActor*>& presented, ESecondaryCookPhase ModifierCookPhase, ESauceType ModifierSauceType)
{
	TArray<FScorePart> Parts;
	Parts.Add({ 100, FText::FromString("Test 1") });
	Parts.Add({ 100, FText::FromString("Test 2") });
	Parts.Add({ 100, FText::FromString("Test 3") });
	return Parts;
}

UDrinkRecipeDataAsset::UDrinkRecipeDataAsset()
{
}

bool UDrinkRecipeDataAsset::CanSubmitStack(const TArray<AActor*>& presented)
{
	return true;
}

TArray<FScorePart> UDrinkRecipeDataAsset::GetScoreFromStack(const TArray<AActor*>& presented, ESecondaryCookPhase ModifierCookPhase, ESauceType ModifierSauceType)
{
	TArray<FScorePart> Parts;
	Parts.Add({ 100, FText::FromString("Test 1") });
	Parts.Add({ 100, FText::FromString("Test 2") });
	Parts.Add({ 100, FText::FromString("Test 3") });
	return Parts;
}
