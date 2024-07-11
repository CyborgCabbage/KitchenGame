// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSchedule.h"

FGameSchedule::FGameSchedule() : Tutorial(false)
{
}

FConfiguredRecipe::FConfiguredRecipe() :
	ModifierCookPhase((ESecondaryCookPhase)FMath::RandRange(1, 3)),
	ModifierSauceType((ESauceType)FMath::RandRange(1, 3))
{
}
