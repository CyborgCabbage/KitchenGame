// Fill out your copyright notice in the Description page of Project Settings.


#include "URecipeManager.h"

URecipeManager::URecipeManager()
{
}

URecipeManager::~URecipeManager()
{
}

void URecipeManager::RegisterIngredient(FString ingredientId, TSubclassOf<AActor> ingredientClass)
{
	IdToClass.Add({ ingredientId, ingredientClass });
}

TSubclassOf<AActor> URecipeManager::TryChop(FString ingredientId)
{
	if (!IsValid(ChopTable)) {
		return nullptr;
	}
	TArray<FCookingRecipe*> rows;
	ChopTable->GetAllRows<FCookingRecipe>("Get Rows for Try Chop", rows);
	for (auto* row : rows) {
		if (row->RecipeInput.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("Chop recipe did not have input"));
			continue;
		}
		if (row->RecipeInput.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("Chop recipe did not have output"));
			continue;
		}
		if (row->RecipeInput[0] == ingredientId) {
			return IdToClass.FindChecked(row->RecipeOutput[0]);
		}
	}
	return nullptr;
}
