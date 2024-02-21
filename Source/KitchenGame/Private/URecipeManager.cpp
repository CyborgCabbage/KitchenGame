// Fill out your copyright notice in the Description page of Project Settings.


#include "URecipeManager.h"
#include "Engine/GameEngine.h"

URecipeManager::URecipeManager()
{
}

URecipeManager::~URecipeManager()
{
}

FTryRecipeResult URecipeManager::TryChop(AActor* ingredient)
{
	InitClassTable();
	FTryRecipeResult result;
	result.Success = false;
	if (!IsValid(ChopTable)) {
		return result;
	}
	TArray<FCookingRecipe*> rows;
	ChopTable->GetAllRows<FCookingRecipe>("TryChop: Get Rows from ChopTable", rows);
	for (auto* row : rows) {
		if (row->RecipeInput.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("TryChop: Recipe did not have input"));
			continue;
		}
		if (row->RecipeInput.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("TryChop: Recipe did not have output"));
			continue;
		}
		if (row->RecipeInput[0] == ingredient->GetClass()->GetName()) {
			if (auto c = NameToClass.Find(row->RecipeOutput[0])) {
				result.ToCreate.Add(*c);
				result.ToDestroy.Add(ingredient);
				result.Success = true;
				return result;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("TryChop: Could not find class for name %s"), *row->RecipeOutput[0]);
				continue;
			}
			
		}
	}
	return result;
}

FTryRecipeResult URecipeManager::TryBlend(const TArray<AActor*>& ingredients)
{
	InitClassTable();
	FTryRecipeResult result;
	result.Success = false;
	if (!IsValid(BlendTable)) {
		return result;
	}
	TArray<FCookingRecipe*> rows;
	BlendTable->GetAllRows<FCookingRecipe>("TryBlend: Get Rows from BlendTable", rows);
	for (auto* row : rows) {
		if (row->RecipeInput.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("TryBlend: Recipe did not have input"));
			continue;
		}
		if (row->RecipeInput.Num() == 0) {
			UE_LOG(LogTemp, Warning, TEXT("TryBlend: Recipe did not have output"));
			continue;
		}
		//Check if ingredients contains row->RecipeInput
		TArray<AActor*> IngredientsConsumable = ingredients;
		TArray<AActor*> ToDestroy;
		bool failed = false;
		for (const auto& rInput : row->RecipeInput) {
			//Get the actor class we are looking for
			TSubclassOf<AActor>* cActor = NameToClass.Find(rInput);
			if (cActor == nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("TryBlend: Could not find class for name %s"), *row->RecipeOutput[0]);
				failed = true;
				break;
			}
			//Find element that matches class
			AActor** Found = IngredientsConsumable.FindByPredicate([&](AActor* Actor) {
				return Actor->GetClass() == *cActor;
			});
			if (Found == nullptr) {
				failed = true;
				break;
			}
			//Update arrays
			ToDestroy.Add(*Found);
			IngredientsConsumable.RemoveSingleSwap(*Found);
		}
		if (failed) {
			continue;
		}
		TArray<TSubclassOf<AActor>> ToCreate;
		for (const auto& rOutput : row->RecipeOutput) {
			//Get the actor class we are returning
			TSubclassOf<AActor>* cActor = NameToClass.Find(rOutput);
			if(cActor == nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("TryBlend: Could not find class for name %s"), *row->RecipeOutput[0]);
				failed = true;
				break;
			}
			//Add to result
			ToCreate.Add(*cActor);
		}
		if (failed) {
			continue;
		}
		result.ToCreate = ToCreate;
		result.ToDestroy = ToDestroy;
		result.Success = true;
		return result;
	}
	return result;
}

bool URecipeManager::TryPresent(const TArray<AActor*>& presented, const FPresentationRecipe& toMatch)
{
	//Convert actors to classes
	TArray<FString> presentedNames;
	for (AActor* Actor : presented) {
		presentedNames.Add(Actor->GetClass()->GetName());
	}
	//Find sub array
	for (int i = 0; i < presentedNames.Num(); i++) {
		for (int j = 0; j < toMatch.ResultStack.Num(); j++) {
			//No more room
			if (i + j >= presentedNames.Num()) {
				return false;
			}
			//Break if they don't match
			if (presentedNames[i + j] != toMatch.ResultStack[j]) {
				break;
			}
			//If we made it to the end confirm match and return true
			if (j == toMatch.ResultStack.Num() - 1) {
				return true;
			}
		}
	}
	return false;
}

void URecipeManager::InitClassTable()
{
	if (NameToClass.Num() > 0) {
		return;
	}
	for (const auto& c : IngredientClasses) {
		if (c) NameToClass.Add({ c->GetName(), c });
	}
}
