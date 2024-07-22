// Fill out your copyright notice in the Description page of Project Settings.


#include "RecipeDataAsset.h"

URecipeDataAsset::URecipeDataAsset() :
	Name(),
	Widget(nullptr),
	Time(60.0f),
	IngredientCount(1)
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
	SecondaryCookPhase(ESecondaryCookPhase::None),
	RequirePlate(true)
{
}

bool UStackRecipeDataAsset::CanSubmitStack(const TArray<AActor*>& presented)
{
	//Get ingredients
	for (AActor* PresentedActor : presented) {
		if (auto* ingredient = Cast<AIngredient>(PresentedActor)) {
			return true;
		}
	}
	return false;
}

enum class MatchStatus {
	Perfect,
	WrongPhase,
	WrongItem
};

static MatchStatus MatchItem(AIngredient* Presented, FRecipeItem Requirement, ESecondaryCookPhase SecondaryCookPhase) {
	bool CorrectItem = (Presented->Data == Requirement.Ingredient);
	if (CorrectItem) {
		//Match phase if it is an ingredient and a phase is specified
		if (Presented->GetPhase().Primary != Requirement.CookPhase) {
			return MatchStatus::WrongPhase;
		}
		if (Requirement.Ingredient->PreciseCooking && Presented->GetPhase().Secondary != SecondaryCookPhase) {
			return MatchStatus::WrongPhase;
		}
		return MatchStatus::Perfect;
	}
	else {
		//Check if the chopped version would be correct
		/*auto ChopResult = TryChop(Presented);
		if (ChopResult.Success) {
			if (ChopResult.ToCreate[0].Get()->GetName() == Requirement.ClassName) {
				return MatchStatus::NotChopped;
			}
		}*/
		return MatchStatus::WrongItem;
	}
}

const int ScoreNoPlate{ -500 };
const int ScoreWrongPhase{ -500 };
const int ScoreExtraItem{ -800 };
const int ScoreOutOfOrder{ -1000 };
const int ScoreIngredient{ 1000 };

static FText PhaseToText(FFullCookPhase CookPhase, bool Precise = false) {
	if (Precise) {
		switch (CookPhase.Secondary) {
		case ESecondaryCookPhase::Rare: return FText::FromString("Rare");
		case ESecondaryCookPhase::Medium: return FText::FromString("Medium");
		case ESecondaryCookPhase::WellDone: return FText::FromString("Well Done");
		}
	}
	switch (CookPhase.Primary) {
	case EPrimaryCookPhase::Raw: return FText::FromString("Raw");
	case EPrimaryCookPhase::Cooking: return FText::FromString("Cooking");
	case EPrimaryCookPhase::Cooked: return FText::FromString("Cooked");
	case EPrimaryCookPhase::Burnt: return FText::FromString("Burnt");
	case EPrimaryCookPhase::Frying: return FText::FromString("Frying");
	case EPrimaryCookPhase::Fried: return FText::FromString("Fried");
	case EPrimaryCookPhase::OverFried: return FText::FromString("OverFried");
	default: return FText::FromString("Unknown");
	}
}

TArray<FScorePart> UStackRecipeDataAsset::GetScoreFromStack(const TArray<AActor*>& presented, ESecondaryCookPhase ModifierCookPhase, ESauceType ModifierSauceType)
{
	ESecondaryCookPhase LocalCookPhase = (ModifierCookPhase == ESecondaryCookPhase::None) ? SecondaryCookPhase : ModifierCookPhase;
	ESauceType LocalSauceType = (ModifierSauceType == ESauceType::None) ? Sauce : ModifierSauceType;
	TArray<FScorePart> Parts;
	if (!presented[0]->ActorHasTag("plate") && RequirePlate) {
		Parts.Add({ ScoreNoPlate, FText::FromString("No plate") });
	}
	//Get ingredients
	TArray<AIngredient*> PresentedIngredients;
	for (AActor* PresentedActor : presented) {
		if (auto* ingredient = Cast<AIngredient>(PresentedActor)) {
			PresentedIngredients.Add(ingredient);
		}
	}
	//Match up
	struct MatchInfo {
		AIngredient* Ref;
		MatchStatus Status;
		int Index;
	};

	TArray<TPair<MatchInfo, FRecipeItem>> Matched;
	for (const FRecipeItem& TM : Items) {
		Matched.Add({
			MatchInfo{nullptr, MatchStatus::WrongItem, -1},
			TM
			});
	}
	for (int i = 0; i < Matched.Num(); i++) {
		for (int j = 0; j < PresentedIngredients.Num(); j++) {
			MatchStatus Status = MatchItem(PresentedIngredients[j], Matched[i].Value, LocalCookPhase);
			if (Status != MatchStatus::WrongItem) {
				Matched[i].Key = { PresentedIngredients[j], Status, j };
				PresentedIngredients.RemoveAt(j);
				break;
			}
		}
	}
	// PresentedIngredients no contains items that were presented but don't correspond to anything in the recipe.
	for(AIngredient* Remaining : PresentedIngredients) {
		Parts.Add({ ScoreExtraItem, FText::Format(FText::FromString("Extra {0}"), Remaining->Data->Name) });
	}
	// MatchedPresented contains presented items that could be matched up to something in the recipe
	int PreviousIndex = -1;
	bool IncorrectOrder = false;
	for (const TPair<MatchInfo, FRecipeItem>& Info : Matched) {
		if (Info.Key.Ref) {
			//Check out of order
			if (Info.Key.Index < PreviousIndex) {
				IncorrectOrder = true;
			}
			PreviousIndex = Info.Key.Index;
			//Add score for ingredient
			Parts.Add({ ScoreIngredient,
					FText::Format(FText::FromString("{0} was presented"),
						Info.Key.Ref->Data->Name
					)
					});

			//Deduct as appropriate
			if (Info.Key.Status == MatchStatus::WrongPhase) {
				FFullCookPhase TargetPhase = Info.Value.CookPhase;
				if (Info.Value.Ingredient->PreciseCooking) {
					TargetPhase.Secondary = LocalCookPhase;
				}
				Parts.Add({ ScoreWrongPhase, 
					FText::Format(FText::FromString("{0} should have been {1} not {2}"), 
						Info.Key.Ref->Data->Name, 
						PhaseToText(TargetPhase, Info.Key.Ref->Data->PreciseCooking),
						PhaseToText(Info.Key.Ref->GetPhase(), Info.Key.Ref->Data->PreciseCooking)
					) 
					});
			}
		}
		else {
			Parts.Add({ 0, FText::Format(FText::FromString("{0} was missing"), Info.Value.Ingredient->Name) });
		}
	}
	if (IncorrectOrder) {
		Parts.Add({ ScoreOutOfOrder, FText::FromString("Out of order") });
	}
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
