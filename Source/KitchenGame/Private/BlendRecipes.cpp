// Fill out your copyright notice in the Description page of Project Settings.


#include "BlendRecipes.h"
#include "AIngredient.h"

bool UBlendRecipes::CheckRecipe(const TArray<AActor*> InputActors, TSubclassOf<AActor>& OutputClass)
{
	//Filter for ingredients
	TArray<TObjectPtr<UIngredientDataAsset>> InputData;
	for (AActor* Actor : InputActors) {
		if(auto* Ingredient = Cast<AIngredient>(Actor)) {
			InputData.Add(Ingredient->Data);
		}
		else {
			return false;
		}
	}
	//Check for match to any of the recipes
	for(const FBlendRecipe& Recipe : BlendRecipes) {
		//Check for each item in the recipe
		TArray<TObjectPtr<UIngredientDataAsset>> InputDataClone = InputData;
		bool FailedToFind = false;
		for (const auto& IngredientData : Recipe.Input) {
			if (InputDataClone.RemoveSingle(IngredientData) == 0) {
				FailedToFind = true;
				break;
			}
		}
		//Did the recipe match?
		if (InputDataClone.IsEmpty() && !FailedToFind) {
			OutputClass = Recipe.Output->Ingredient;
			return true;
		}
	}
	return false;
}
