// Fill out your copyright notice in the Description page of Project Settings.


#include "URecipeManager.h"
#include "Engine/GameEngine.h"
/*
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

URecipeManager::MatchRequirement URecipeManager::ParseMatchString(const FString& String)
{
	FJsonSerializableArray MatchTerms;
	String.ParseIntoArray(MatchTerms, TEXT(","));
	return { MatchTerms[0], MatchTerms.IsValidIndex(1) ? TOptional{MatchTerms[1]} : NullOpt };
}

URecipeManager::MatchStatus URecipeManager::MatchItem(AIngredient* Presented, MatchRequirement Requirement)
{
	FString ActorName = Presented->GetClass()->GetName();
	//Parse match string
	
	bool CorrectItem = (ActorName == Requirement.ClassName);
	if (CorrectItem) {
		//Match phase if it is an ingredient and a phase is specified
		if (!Requirement.Phase) {
			return MatchStatus::Perfect;
		}
		if (Presented->GetPhase().Primary == Requirement.Phase.GetValue()) {
			return MatchStatus::Perfect;
		}
		else {
			return MatchStatus::WrongPhase;
		}
	}
	else{
		//Check if the chopped version would be correct
		auto ChopResult = TryChop(Presented);
		if (ChopResult.Success) {
			if (ChopResult.ToCreate[0].Get()->GetName() == Requirement.ClassName) {
				return MatchStatus::NotChopped;
			}
		}
		return MatchStatus::WrongItem;
	}
}

const int DeductNoPlate{ 500 };
const int DeductWrongPhase{ 1000 };
const int DeductNotChopped{ 2000 };
const int DeductMissingItem{ 2500 };
const int DeductExtraItem{ 2000 };
const int DeductOutOfOrder{ 1000 };

TArray<FScoreDeduction> URecipeManager::TryPresent(const TArray<AActor*>& presented, const FPresentationRecipe& toMatch)
{
	
	TArray<FScoreDeduction> Deductions;
	FString ActorName = presented[0]->GetClass()->GetName();
	if (ActorName != "BP_Plate_C") {
		Deductions.Add({ DeductNoPlate, TEXT("No plate") });
	}
	//Get ingredients
	TArray<AIngredient*> PresentedIngredients;
	for (AActor* PresentedActor : presented) {
		auto* ingredient = Cast<AIngredient>(PresentedActor);
		if (ingredient) {
			PresentedIngredients.Add(ingredient);
		}
	}
	//Match up
	
	struct MatchInfo {
		AIngredient* Ref;
		MatchStatus Status;
		int Index;
	};
	
	TArray<TPair<MatchInfo, MatchRequirement>> Matched;
	for (const auto& TM : toMatch.ResultStack) {
		Matched.Add({ 
			MatchInfo{nullptr, MatchStatus::WrongItem, -1}, 
			ParseMatchString(TM)
		});
	}
	for (int i = 0; i < Matched.Num(); i++) {
		for (int j = 0; j < PresentedIngredients.Num(); j++) {
			MatchStatus Status = MatchItem(PresentedIngredients[j], Matched[i].Value);
			if (Status != MatchStatus::WrongItem) {
				Matched[i].Key = { PresentedIngredients[j], Status, j};
				PresentedIngredients.RemoveAt(j);
				break;
			}
		}
	}
	// PresentedIngredients no contains items that were presented but don't correspond to anything in the recipe.
	if (!PresentedIngredients.IsEmpty()) {
		Deductions.Add({ DeductExtraItem, TEXT("Extra ingredient(s)") });
	}
	// MatchedPresented contains presented items that could be matched up to something in the recipe
	int PreviousIndex = -1;
	bool IncorrectOrder = false;
	for (const TPair<MatchInfo, MatchRequirement>& Info : Matched) {
		if (Info.Key.Ref) {
			//Check out of order
			if (Info.Key.Index < PreviousIndex) {
				IncorrectOrder = true;
			}
			PreviousIndex = Info.Key.Index;
			//Deduct as appropriate
			if (Info.Key.Status == MatchStatus::WrongPhase) Deductions.Add({ DeductWrongPhase, FString::Printf(TEXT("'%s' should have been '%s' not '%s'"), *Info.Key.Ref->IngredientID, *Info.Value.Phase.GetValue(), *Info.Key.Ref->GetPhase())});
			if (Info.Key.Status == MatchStatus::NotChopped) Deductions.Add({ DeductNotChopped, FString::Printf(TEXT("'%s' should have been chopped"), *Info.Key.Ref->IngredientID) });
		}
		else {
			FString IngredientName = Info.Value.ClassName;
			InitClassTable();
			if (auto* IngredientClass = NameToClass.Find(Info.Value.ClassName)) {
				if (AIngredient* CDO = Cast<AIngredient>(IngredientClass->GetDefaultObject())) {
					IngredientName = CDO->IngredientID;
				}
			}
			Deductions.Add({ DeductMissingItem, FString::Printf(TEXT("'%s' was missing"), *IngredientName) });
		}
	}
	if (IncorrectOrder) {
		Deductions.Add({ DeductOutOfOrder, TEXT("Out of order") });
	}
	return Deductions;
}

int URecipeManager::GetMaxDeductions(const FPresentationRecipe& toMatch)
{
	return DeductExtraItem + DeductMissingItem * toMatch.ResultStack.Num();
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
*/