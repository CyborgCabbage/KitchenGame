// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientDataAsset.h"
#include "AIngredient.h"

UIngredientDataAsset::UIngredientDataAsset() :
	Name(),
	Ingredient(nullptr),
	Sprite(nullptr),
	Color(1.0f, 1.0f, 1.0f),
	SauceOutput(ESauceType::None),
	CookTime(10),
	CookedTime(10),
	BurnTime(10),
	FryTime(10),
	FriedTime(10),
	OverFryTime(10),
	PreciseCooking(false),
	RawColor(1, 1, 1),
	CookColor(0.7f, 0.7f, 0.7f),
	BurnColor(0, 0, 0),
	FryColor(1.0f, 0.4f, 0.04f),
	OverFryColor(0.2f, 0.1f, 0)
{
}
