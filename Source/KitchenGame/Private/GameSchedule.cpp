// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSchedule.h"

FGameSchedule::FGameSchedule()
{
}

FConfiguredRecipe::FConfiguredRecipe() :
	RecipeAsset(nullptr),
	ModifierCookPhase(ESecondaryCookPhase::None),
	ModifierSauceType(ESauceType::None),
	Tutorial(false)
{
}